module;

#include <VulkanHeader.h>

module Coral.Vulkan.Fence;

import Coral.Vulkan.Context;

using namespace Coral::Vulkan;


Fence::~Fence()
{
	if (mContext && mFence != VK_NULL_HANDLE)
	{
		vkDestroyFence(mContext->getVkDevice(), mFence, nullptr);
	}
}


bool
Fence::init(Context* context)
{
	mContext = context;
	VkFenceCreateInfo createInfo{ VK_STRUCTURE_TYPE_FENCE_CREATE_INFO };
	
	return vkCreateFence(context->getVkDevice(), &createInfo, nullptr, &mFence) == VK_SUCCESS;
}


VkFence
Fence::getVkFence()
{
	return mFence;
}


bool
Fence::wait()
{
	return vkWaitForFences(mContext->getVkDevice(), 1, &mFence, VK_TRUE, UINT64_MAX) == VK_SUCCESS;
}


void
Fence::reset()
{
	vkResetFences(mContext->getVkDevice(), 1, &mFence);
}
