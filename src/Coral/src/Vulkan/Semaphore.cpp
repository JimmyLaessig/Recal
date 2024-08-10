module;

#include <VulkanHeader.h>

module Coral.Vulkan.Semaphore;

import Coral.Vulkan.Context;

using namespace Coral::Vulkan;


bool
Semaphore::init(Coral::Vulkan::Context* context)
{
	mContext = context;
	VkSemaphoreTypeCreateInfo timelineCreateInfo{ VK_STRUCTURE_TYPE_SEMAPHORE_TYPE_CREATE_INFO };
	timelineCreateInfo.pNext			= NULL;
	timelineCreateInfo.semaphoreType	= VK_SEMAPHORE_TYPE_TIMELINE;
	timelineCreateInfo.initialValue		= 0;

	VkSemaphoreCreateInfo info{ VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO };
	info.pNext = &timelineCreateInfo;
	return vkCreateSemaphore(mContext->getVkDevice(), &info, nullptr, &mSemaphore) == VK_SUCCESS;
}


Semaphore::~Semaphore()
{
	if (mSemaphore != VK_NULL_HANDLE)
	{
		vkDestroySemaphore(mContext->getVkDevice(), mSemaphore, nullptr);
	}
}


VkSemaphore
Semaphore::getVkSemaphore()
{
	return mSemaphore;
}


bool
Semaphore::wait(uint64_t timeout)
{
	constexpr static uint64_t ONE = 1;
	VkSemaphoreWaitInfo waitInfo{ VK_STRUCTURE_TYPE_SEMAPHORE_WAIT_INFO };
	waitInfo.semaphoreCount = 1;
	waitInfo.pValues		= &ONE;
	waitInfo.pSemaphores	= &mSemaphore;
	return vkWaitSemaphores(mContext->getVkDevice(), &waitInfo, timeout) == VK_SUCCESS;
}