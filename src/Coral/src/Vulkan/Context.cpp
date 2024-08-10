module;

#define VMA_IMPLEMENTATION
#include <VulkanHeader.h>

#include <array>
#include <cassert>
#include <chrono>
#include <memory>
#include <mutex>

module Coral.Vulkan.Context;

import Coral.Vulkan.Buffer;
import Coral.Vulkan.CommandQueue;
import Coral.Vulkan.Surface;
import Coral.Vulkan.SurfaceWin32;

import Coral.Vulkan.Semaphore;

//import Log20;

using namespace Coral::Vulkan;

Context::~Context()
{
	mTransferQueue.reset();	 
	mGraphicsQueue.reset();
	mComputeQueue.reset();
	mStagingBufferPool.reset();

	if (mDescriptorPool != VK_NULL_HANDLE)
	{
		vkDestroyDescriptorPool(mDevice, mDescriptorPool, nullptr);
	}

	if (mAllocator != VK_NULL_HANDLE)
	{
		vmaDestroyAllocator(mAllocator);
	}

	if (mDevice != VK_NULL_HANDLE)
	{
		vkDestroyDevice(mDevice, nullptr);
	}

	if (mDebugMessenger)
	{
		vkb::destroy_debug_utils_messenger(mInstance, mDebugMessenger, nullptr);
	}

	if (mInstance != VK_NULL_HANDLE)
	{
		vkDestroyInstance(mInstance, nullptr);
	}
}


bool
Context::init(const ContextConfig& config)
{
	//Log::info("Initializing Vulkan Rendering Context...");

	if (volkInitialize() != VK_SUCCESS)
	{
		return false;
	}

	vkb::InstanceBuilder builder;
	auto instance = builder.set_app_name("Engine")
		.request_validation_layers(true)
		.use_default_debug_messenger()
		.require_api_version(1, 3, 0)
		.build();

	if (!instance)
	{
		return false;
	}

	mInstance		= instance->instance;

	volkLoadInstance(mInstance);

	mDebugMessenger = instance->debug_messenger;

	// Vulkan 1.0 features
	VkPhysicalDeviceFeatures features10{};
	features10.fillModeNonSolid = VK_TRUE;

	// Vulkan 1.1 features
	VkPhysicalDeviceVulkan11Features features11{};

	// Vulkan 1.2 features
	VkPhysicalDeviceVulkan12Features features12{};
	features12.bufferDeviceAddress = VK_TRUE;
	features12.timelineSemaphore = VK_TRUE;

	// Vulkan 1.3 features
	VkPhysicalDeviceVulkan13Features features13{};
	features13.dynamicRendering = VK_TRUE;
	features13.synchronization2 = VK_TRUE;

	// use vkbootstrap to select a GPU. 
	// We want a GPU that can write to the surface and supports Vulkan 1.3 with the correct features
	vkb::PhysicalDeviceSelector selector{ instance.value()};
	auto physicalDevice = selector
		.set_minimum_version(1, 3)
		.set_required_features(features10)
		.set_required_features_11(features11)
		.set_required_features_12(features12)
		.set_required_features_13(features13)
		.defer_surface_initialization()
		.add_required_extension(VK_EXT_NESTED_COMMAND_BUFFER_EXTENSION_NAME)
		.select();

	if (!physicalDevice)
	{
		//Log::error("Failed to select physical device: {}", physicalDevice.error().message());
		return false;
	}

	mPhysicalDevice = physicalDevice->physical_device;

	std::optional<uint32_t> queueFamilyIndex;
	// Look for a device queue family that supports GRAPHICS, COMPUTE and 
	// TRANSFER in one, so we don't need command pool for different queue
	// families. Ideally, the queue family also for a dedicated queue for each
	// queue type.
	uint32_t index{ 0 };
	auto queueFamilies = physicalDevice->get_queue_families();
	for (const auto& queueFamily : queueFamilies)
	{
		// We don't need to check for VK_QUEUE_TRANSFER_BIT explicitely.
		// 
		// All commands that are allowed on a queue that supports transfer 
		// operations are also allowed on a queue that supports either graphics 
		// or compute operations.Thus, if the capabilities of a queue family 
		// include VK_QUEUE_GRAPHICS_BIT or VK_QUEUE_COMPUTE_BIT, then 
		// reporting the VK_QUEUE_TRANSFER_BIT capability separately for that 
		// queue family is optional.
		auto flags = VK_QUEUE_GRAPHICS_BIT | VK_QUEUE_COMPUTE_BIT;

		auto isSuitable = (queueFamily.queueFlags & flags) == flags;

		if (!isSuitable)
		{
			continue;
		}

		queueFamilyIndex = index;

		// We found a queue family that supports three or more queues. We
		// consider this as optimal so we can have dedicated GRAPHICS, COMPUTE 
		// and TRANSFER queues.
		if (queueFamily.queueCount > 2)
		{
			break;
		}
		index++;
	}

	if (!queueFamilyIndex)
	{
		return false;
	}

	mQueueFamilyIndex = *queueFamilyIndex;

	auto numDedicatedQueues = std::min(static_cast<uint32_t>(queueFamilies.size()), 3u);

	std::vector<float> priorities(numDedicatedQueues, 1.f);

	vkb::CustomQueueDescription queueDescription{ mQueueFamilyIndex, numDedicatedQueues, priorities };

	vkb::DeviceBuilder deviceBuilder{ physicalDevice.value()};
	auto device = deviceBuilder.custom_queue_setup({ queueDescription })
							   .build();

	if (!device)
	{
		return false;
	}

	mDevice = device->device;
	volkLoadDevice(mDevice);

	switch (numDedicatedQueues)
	{
		case 1:
		{
			VkQueue queue{ VK_NULL_HANDLE };
			vkGetDeviceQueue(mDevice, mQueueFamilyIndex, 0, &queue);

			mGraphicsQueue	= std::make_shared<Coral::Vulkan::CommandQueue>(this, queue, 0, mQueueFamilyIndex);
			mComputeQueue	= mGraphicsQueue;
			mTransferQueue	= mGraphicsQueue;
			break;
		}
		case 2:
		{
			VkQueue queue0{ VK_NULL_HANDLE };
			VkQueue queue1{ VK_NULL_HANDLE };
			vkGetDeviceQueue(mDevice, mQueueFamilyIndex, 0, &queue0);
			vkGetDeviceQueue(mDevice, mQueueFamilyIndex, 1, &queue1);

			mGraphicsQueue	= std::make_shared<Coral::Vulkan::CommandQueue>(this, queue0, 0, mQueueFamilyIndex);
			mComputeQueue	= mGraphicsQueue;
			mTransferQueue	= std::make_shared<Coral::Vulkan::CommandQueue>(this, queue1, 1, mQueueFamilyIndex);
			break;
		}
		case 3:
		{
			VkQueue queue0{ VK_NULL_HANDLE };
			VkQueue queue1{ VK_NULL_HANDLE };
			VkQueue queue2{ VK_NULL_HANDLE };
			vkGetDeviceQueue(mDevice, mQueueFamilyIndex, 0, &queue0);
			vkGetDeviceQueue(mDevice, mQueueFamilyIndex, 1, &queue1);
			vkGetDeviceQueue(mDevice, mQueueFamilyIndex, 2, &queue2);

			mGraphicsQueue	= std::make_shared<Coral::Vulkan::CommandQueue>(this, queue0, 0, mQueueFamilyIndex);
			mComputeQueue	= std::make_shared<Coral::Vulkan::CommandQueue>(this, queue1, 1, mQueueFamilyIndex);
			mTransferQueue	= std::make_shared<Coral::Vulkan::CommandQueue>(this, queue2, 2, mQueueFamilyIndex);
			break;
		}
		default:
			assert(false);
			return false;
	}

	VmaAllocatorCreateInfo allocatorCreateInfo{};
	allocatorCreateInfo.device				= mDevice;
	allocatorCreateInfo.instance			= mInstance;
	allocatorCreateInfo.physicalDevice		= mPhysicalDevice;
	allocatorCreateInfo.vulkanApiVersion	= VK_API_VERSION_1_3;

	VmaVulkanFunctions functions{};
	functions.vkGetInstanceProcAddr = vkGetInstanceProcAddr;
	functions.vkGetDeviceProcAddr = vkGetDeviceProcAddr;
	allocatorCreateInfo.pVulkanFunctions = &functions;

	//allocatorCreateInfo.flags				= VMA_ALLOCATOR_CREATE_BUFFER_DEVICE_ADDRESS_BIT;
	// Allocate 64 MiB sized memory block chunks. If requested, the allocator 
	// will create larger memory blocks to ensure continuous memory per buffer.
	allocatorCreateInfo.preferredLargeHeapBlockSize = 1024 * 1024 * 64;
	if (vmaCreateAllocator(&allocatorCreateInfo, &mAllocator) != VK_SUCCESS)
	{
		return false;
	}

	std::array<VkDescriptorPoolSize, 3> poolSizes 
	{
		VkDescriptorPoolSize{ VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER , 32u },
		VkDescriptorPoolSize{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER , 32u },
		VkDescriptorPoolSize{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER , 32u },
	};
	
	VkDescriptorPoolCreateInfo descriptorPoolCreateInfo{ VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO };
	descriptorPoolCreateInfo.poolSizeCount	= static_cast<uint32_t>(poolSizes.size());
	descriptorPoolCreateInfo.pPoolSizes		= poolSizes.data();
	descriptorPoolCreateInfo.maxSets		= 256;

	if (vkCreateDescriptorPool(mDevice, &descriptorPoolCreateInfo, nullptr, &mDescriptorPool) != VK_SUCCESS)
	{
		return false;
	}

	mStagingBufferPool = std::make_unique<BufferPool>(*this, Coral::BufferType::STORAGE_BUFFER, true);

	return true;
}


Coral::CommandQueue*
Context::getGraphicsQueue()
{
	return mGraphicsQueue.get();
}


Coral::CommandQueue*
Context::getTransferQueue()
{
	return mTransferQueue.get();
}


Coral::CommandQueue*
Context::getComputeQueue()
{
	return mComputeQueue.get();
}


VmaAllocator 
Context::getVmaAllocator()
{
	return mAllocator;
}


VkDescriptorPool
Context::getVkDescriptorPool()
{
	return mDescriptorPool;
}


uint32_t 
Context::getQueueFamilyIndex()
{
	return mQueueFamilyIndex;
}


std::unique_ptr<Coral::Vulkan::Buffer>
Context::requestStagingBuffer(size_t bufferSize)
{
	return mStagingBufferPool->requestBuffer(bufferSize);
}


void
Context::returnStagingBuffersAfterUse(std::vector<std::unique_ptr<Coral::Vulkan::Buffer>>&& stagingBuffers,
									  std::unique_ptr<Coral::Vulkan::Semaphore>&& waitSemaphore)
{
	mStagingBufferPool->returnBuffersAfterUse(std::forward<std::vector<std::unique_ptr<Coral::Vulkan::Buffer>>>(stagingBuffers), 
										     std::forward<std::unique_ptr<Coral::Vulkan::Semaphore>>(waitSemaphore));
}
