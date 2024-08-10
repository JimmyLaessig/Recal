module;

#include <VulkanHeader.h>

#include <mutex>
#include <vector>

module Coral.Vulkan.CommandQueue;

import Coral.CommandBuffer;

import Coral.RAII;
import Coral.Vulkan.Context;
import Coral.Vulkan.CommandBuffer;
import Coral.Vulkan.Semaphore;
import Coral.Surface;
import Coral.Vulkan.Surface;
import Coral.Fence;
import Coral.Vulkan.Fence;

using namespace Coral::Vulkan;

CommandQueue::CommandQueue(Context* context, VkQueue queue, uint32_t queueIndex, uint32_t queueFamilyIndex)
	: mContext(context)
	, mQueue(queue)
	, mQueueIndex(queueIndex)
	, mQueueFamilyIndex(queueFamilyIndex)
{
}


CommandQueue::~CommandQueue()
{
	if (!mContext)
	{
		return;
	}

	for (auto [_, commandPool] : mCommandPools)
	{
		vkDestroyCommandPool(mContext->getVkDevice(), commandPool, nullptr);
	}
}


bool
CommandQueue::submit(const Coral::CommandBufferSubmitInfo& info, Coral::Fence* fence)
{
	std::lock_guard lock(mQueueProtection);

	std::vector<VkSemaphore> waitSemaphores;
	std::vector<VkPipelineStageFlags> waitFlags;
	std::vector<uint64_t> waitValues;
	for (auto semaphore : info.waitSemaphores)
	{
		waitSemaphores.push_back(static_cast<Vulkan::Semaphore*>(semaphore)->getVkSemaphore());
		// Wait with execution of all commands until all semaphores are signaled
		waitFlags.push_back(VK_PIPELINE_STAGE_ALL_COMMANDS_BIT);
		waitValues.push_back(1);
	}

	std::vector<VkSemaphore> signalSemaphores;
	std::vector<uint64_t> signalValues;
	for (auto semaphore : info.signalSemaphores)
	{
		signalSemaphores.push_back(static_cast<Vulkan::Semaphore*>(semaphore)->getVkSemaphore());
		signalValues.push_back(1);
	}

	std::vector<std::unique_ptr<Coral::Vulkan::Buffer>> stagingBuffers;

	std::vector<VkCommandBuffer> commandBuffers;
	for (auto commandBuffer : info.commandBuffers)
	{
		auto commandBufferImpl = static_cast<Vulkan::CommandBuffer*>(commandBuffer);
		commandBuffers.push_back(commandBufferImpl->getVkCommandBuffer());

		auto commandBufferStagingBuffers = commandBufferImpl->getStagingBuffers();

		stagingBuffers.insert(stagingBuffers.end(), 
							  std::move_iterator(commandBufferStagingBuffers.begin()), 
							  std::move_iterator(commandBufferStagingBuffers.end()));
	}

	std::unique_ptr<Coral::Vulkan::Semaphore> stagingBufferSemaphore;
	if (!stagingBuffers.empty())
	{
		stagingBufferSemaphore = std::unique_ptr<Coral::Vulkan::Semaphore>(static_cast<Coral::Vulkan::Semaphore*>(Coral::createSemaphore(mContext)));

		signalSemaphores.push_back(stagingBufferSemaphore->getVkSemaphore());
		signalValues.push_back(1);
	}

	VkTimelineSemaphoreSubmitInfo timelineInfo{ VK_STRUCTURE_TYPE_TIMELINE_SEMAPHORE_SUBMIT_INFO };
	timelineInfo.pNext						= nullptr;
	timelineInfo.waitSemaphoreValueCount	= static_cast<uint32_t>(waitValues.size());
	timelineInfo.pWaitSemaphoreValues		= waitValues.data();
	timelineInfo.signalSemaphoreValueCount	= static_cast<uint32_t>(signalValues.size());
	timelineInfo.pSignalSemaphoreValues		= signalValues.data();

	VkSubmitInfo submitInfo{ VK_STRUCTURE_TYPE_SUBMIT_INFO };
	submitInfo.pNext				= &timelineInfo;
	submitInfo.pCommandBuffers		= commandBuffers.data();
	submitInfo.commandBufferCount	= static_cast<uint32_t>(commandBuffers.size());
	submitInfo.pSignalSemaphores	= signalSemaphores.data();
	submitInfo.signalSemaphoreCount = static_cast<uint32_t>(signalSemaphores.size());
	submitInfo.pWaitSemaphores		= waitSemaphores.data();
	submitInfo.waitSemaphoreCount	= static_cast<uint32_t>(waitSemaphores.size());
	submitInfo.pWaitDstStageMask	= waitFlags.data();

	VkFence vkFence = fence ? static_cast<Coral::Vulkan::Fence*>(fence)->getVkFence() : VK_NULL_HANDLE;

	if (vkQueueSubmit(mQueue, 1, &submitInfo, vkFence) != VK_SUCCESS)
	{
		return false;
	}

	if (stagingBufferSemaphore)
	{
		mContext->returnStagingBuffersAfterUse(std::move(stagingBuffers), std::move(stagingBufferSemaphore));
	}
	
	return true;
}


bool
CommandQueue::waitIdle()
{
	return vkQueueWaitIdle(mQueue) == VK_SUCCESS;
}


bool 
CommandQueue::submit(const Coral::PresentInfo& info)
{
	std::vector<VkSemaphore> waitSemaphores;
	for (auto semaphore : info.waitSemaphores)
	{
		waitSemaphores.push_back(static_cast<Vulkan::Semaphore*>(semaphore)->getVkSemaphore());
	}

	VkPresentInfoKHR presentInfo{ VK_STRUCTURE_TYPE_PRESENT_INFO_KHR };
	presentInfo.pWaitSemaphores		= waitSemaphores.data();
	presentInfo.waitSemaphoreCount	= static_cast<uint32_t>(waitSemaphores.size());

	uint32_t swapchainImageIndex = info.surface->getCurrentSwapchainImageIndex();
	presentInfo.pImageIndices = &swapchainImageIndex;

	VkSwapchainKHR swapchain = static_cast<Coral::Vulkan::Surface*>(info.surface)->getVkSwapchain();
	presentInfo.pSwapchains = &swapchain;
	presentInfo.swapchainCount = 1;
	
	return vkQueuePresentKHR(mQueue, &presentInfo);
}


VkCommandPool
CommandQueue::getVkCommandPool()
{
	std::lock_guard lock(mQueueProtection);

	auto id = std::this_thread::get_id();

	auto iter = mCommandPools.find(id);

	if (iter != mCommandPools.end())
	{
		return iter->second;
	}

	VkCommandPoolCreateInfo createInfo{ VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO };
	createInfo.queueFamilyIndex = mQueueFamilyIndex;
	createInfo.flags			= VK_COMMAND_POOL_CREATE_TRANSIENT_BIT | VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

	VkCommandPool commandPool{ VK_NULL_HANDLE };
	if (vkCreateCommandPool(mContext->getVkDevice(), &createInfo, nullptr, &commandPool) != VK_SUCCESS)
	{
		return VK_NULL_HANDLE;
	}

	mCommandPools[id] = commandPool;

	return commandPool;
}


VkQueue
CommandQueue::getVkQueue()
{
	return mQueue;
}