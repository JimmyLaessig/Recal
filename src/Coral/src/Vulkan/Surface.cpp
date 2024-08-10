module;

#include <VulkanHeader.h>

#include <algorithm>
#include <memory>
#include <vector>

module Coral.Vulkan.Surface;

import Coral.Fence;

import Coral.Vulkan.Fence;
import Coral.Vulkan.Context;
import Coral.Vulkan.Format;
import Coral.Vulkan.Framebuffer;

import Coral.Vulkan.SurfaceWin32;

using namespace Coral::Vulkan;

namespace
{

VkSurfaceFormatKHR
chooseSwapchainFormat(VkPhysicalDevice device, VkSurfaceKHR surface)
{
	uint32_t formatCount{ 0 };
	vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);
	std::vector<VkSurfaceFormatKHR> formats(formatCount);
	vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, formats.data());

	for (auto format : formats)
	{
		if (format.format == VK_FORMAT_R8G8B8A8_SRGB && format.colorSpace == VK_COLORSPACE_SRGB_NONLINEAR_KHR)
		{
			return format;
		}
	}
	return formats.front();
}


	
} // namespace

Surface::~Surface()
{
	if (mSwapchain)
	{
		vkDestroySwapchainKHR(mContext->getVkDevice(), mSwapchain, nullptr);
	}

	if (mSurface)
	{
		vkDestroySurfaceKHR(mContext->getVkInstance(), mSurface, nullptr);
	}
}


bool
Surface::initSwapchain(const Coral::SwapchainConfig& config)
{
	mConfig = config;

	mSwapchainImageData.clear();
	mSwapchainDepthImage.reset();
	auto surfaceFormat = chooseSwapchainFormat(mContext->getVkPhysicalDevice(), mSurface);

	VkSurfaceCapabilitiesKHR surfaceCapabilites{ };
	if (vkGetPhysicalDeviceSurfaceCapabilitiesKHR(mContext->getVkPhysicalDevice(), mSurface, &surfaceCapabilites) != VK_SUCCESS)
	{
		return false;
	}

	VkExtent2D swapchainExtent;
	swapchainExtent.width = std::clamp(surfaceCapabilites.currentExtent.width, surfaceCapabilites.minImageExtent.width, surfaceCapabilites.maxImageExtent.width);
	swapchainExtent.height = std::clamp(surfaceCapabilites.currentExtent.height, surfaceCapabilites.minImageExtent.height, surfaceCapabilites.maxImageExtent.height);

	VkSwapchainCreateInfoKHR createInfo{ VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR };
	createInfo.surface			= mSurface;
	createInfo.minImageCount	= config.swapchainImageCount;
	createInfo.imageFormat		= surfaceFormat.format;
	createInfo.imageColorSpace	= surfaceFormat.colorSpace;
	createInfo.imageExtent		= swapchainExtent;
	createInfo.imageArrayLayers = 1;
	createInfo.imageUsage		= VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
	createInfo.presentMode		= VK_PRESENT_MODE_FIFO_KHR;
	createInfo.oldSwapchain		= mSwapchain;
	createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
	createInfo.preTransform		= surfaceCapabilites.currentTransform;
	createInfo.compositeAlpha	= VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;

	if (vkCreateSwapchainKHR(mContext->getVkDevice(), &createInfo, nullptr, &mSwapchain) != VK_SUCCESS)
	{
		return false;
	}

	// Query the number of swapchain images
	if (vkGetSwapchainImagesKHR(mContext->getVkDevice(), mSwapchain, &mSwapchainImageCount, nullptr) != VK_SUCCESS)
	{
		return false;
	}

	// Query the handles to the swapchain images
	std::vector<VkImage> swapchainImages(mSwapchainImageCount);
	if (vkGetSwapchainImagesKHR(mContext->getVkDevice(), mSwapchain, &mSwapchainImageCount, swapchainImages.data()) != VK_SUCCESS)
	{
		return false;
	}

	// Create image views
	mSwapchainImageData.resize(mSwapchainImageCount);
	for (size_t i = 0; i < mSwapchainImageCount; ++i)
	{
		auto image = std::make_unique<Coral::Vulkan::Image>();

		// Store the swapchain image
		if (!image->init(mContext,
			swapchainImages[i],
			convert(createInfo.imageFormat),
			swapchainExtent.width,
			swapchainExtent.height,
			1,
			VK_IMAGE_LAYOUT_PRESENT_SRC_KHR))
		{
			return false;
		}

		mSwapchainImageData[i].image = std::move(image);
	}

	// Create the depth buffer
	if (config.depthFormat)
	{
		auto depthFormat = Coral::Vulkan::convert(*config.depthFormat);

		if (!isDepthFormat(*config.depthFormat))
		{
			return false;
		}

		Coral::ImageConfig depthImageConfig{};
		depthImageConfig.format = *config.depthFormat;
		depthImageConfig.width = createInfo.imageExtent.width;
		depthImageConfig.height = createInfo.imageExtent.height;

		mSwapchainDepthImage = std::make_unique<Coral::Vulkan::Image>();
		if (!mSwapchainDepthImage->init(mContext, depthImageConfig))
		{
			return false;
		}
	}

	// Create the Framebuffer for each swapchain image
	for (size_t i = 0; i < mSwapchainImageCount; ++i)
	{
		auto framebuffer = std::make_unique<Coral::Vulkan::Framebuffer>();

		Coral::FramebufferConfig framebufferConfig{};

		Coral::ColorAttachment colorAttachment{ mSwapchainImageData[i].image.get(), Coral::ClearOp::CLEAR };

		framebufferConfig.colorAttachment = { &colorAttachment, 1 };

		if (config.depthFormat)
		{
			framebufferConfig.depthAttachment = { mSwapchainDepthImage.get(), Coral::ClearOp::CLEAR };
		}

		if (!framebuffer->init(mContext, framebufferConfig))
		{
			return false;
		}

		mSwapchainImageData[i].framebuffer = std::move(framebuffer);
	}

	return true;
}


bool
Surface::init(Coral::Vulkan::Context* context, const Coral::SurfaceConfig& config)
{
	mContext = context;

	mSurface = Coral::Vulkan::createVkSurface(context->getVkInstance(), config.nativeWindowHandle);

	if (mSurface == VK_NULL_HANDLE)
	{
		return false;
	}

	return initSwapchain(config.swapchainConfig);
}


VkSurfaceKHR
Surface::getVkSurface()
{
	return mSurface;
}


VkSwapchainKHR
Surface::getVkSwapchain()
{
	return mSwapchain;
}


void*
Surface::nativeWindowHandle()
{
	return mNativeWindowHandle;
}


bool
Surface::acquireNextSwapchainImage(Coral::Fence* fence)
{
	uint32_t swapchainIndex{ 0 };

	VkFence vulkanFence = fence ? static_cast<Coral::Vulkan::Fence*>(fence)->getVkFence() : VK_NULL_HANDLE;

	auto result = vkAcquireNextImageKHR(mContext->getVkDevice(), mSwapchain, UINT32_MAX, VK_NULL_HANDLE, vulkanFence, &mCurrentSwapchainIndex);

	// check if VK_ERROR_OUT_OF_DATE_KHR, then rebuild the swapchain
	if (result == VK_ERROR_OUT_OF_DATE_KHR)
	{
		vkDeviceWaitIdle(mContext->getVkDevice());

		if (!initSwapchain(mConfig))
		{
			return false;
		}

		return acquireNextSwapchainImage(fence);
	}

	return result == VK_SUCCESS;
}


uint32_t
Surface::getCurrentSwapchainImageIndex()
{
	return mCurrentSwapchainIndex;
}


Coral::Image*
Surface::getSwapchainImage(uint32_t index)
{
	if (index >= mSwapchainImageData.size())
	{
		return nullptr;
	}

	return mSwapchainImageData[index].image.get();
}


Coral::Image*
Surface::getSwapchainDepthImage()
{
	return mSwapchainDepthImage.get();
}


Coral::Framebuffer*
Surface::getSwapchainFramebuffer(uint32_t index)
{
	if (index >= mSwapchainImageData.size())
	{
		return nullptr;
	}
	return mSwapchainImageData[index].framebuffer.get();
}
