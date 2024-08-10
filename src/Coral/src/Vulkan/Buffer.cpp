module;

#include <VulkanHeader.h>

#include <cassert>
#include <string>
#include <span>

module Coral.Vulkan.Buffer;

import Coral.Vulkan.Context;
import Coral.RAII;
import Coral.CommandQueue;
import Coral.CommandBuffer;

using namespace Coral::Vulkan;


bool
Buffer::init(Coral::Vulkan::Context* context,
			 const Coral::BufferConfig& config)
{
	if (!context)
	{
		return false;
	}

	mContext	= context;
	mType		= config.type;
	mSize		= config.size;
	mCpuVisible = config.cpuVisible;

	uint32_t queueFamilyIndex = context->getQueueFamilyIndex();

	VkBufferCreateInfo createInfo{ VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO };
	createInfo.pQueueFamilyIndices		= &queueFamilyIndex;
	createInfo.queueFamilyIndexCount	= 1;
	createInfo.sharingMode				= VK_SHARING_MODE_EXCLUSIVE;
	createInfo.size						= mSize;

	switch (mType)
	{
		case Coral::BufferType::INDEX_BUFFER:
			createInfo.usage = VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;
			break;
		case Coral::BufferType::VERTEX_BUFFER:
			createInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;
			break;
		case Coral::BufferType::STORAGE_BUFFER:
			createInfo.usage = VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_SRC_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;
			break;
		case Coral::BufferType::UNIFORM_BUFFER:
			createInfo.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;
		default:
			break;
	}

	VmaAllocationCreateInfo allocCreateInfo{};
	allocCreateInfo.usage = VMA_MEMORY_USAGE_AUTO;

	if (mCpuVisible)
	{
		allocCreateInfo.flags = VMA_ALLOCATION_CREATE_MAPPED_BIT | VMA_ALLOCATION_CREATE_HOST_ACCESS_RANDOM_BIT;
	}

	VmaAllocationInfo allocInfo{};

	if (vmaCreateBuffer(context->getVmaAllocator(), &createInfo, &allocCreateInfo, &mBuffer, &mAllocation, &allocInfo) != VK_SUCCESS)
	{
		return false;
	}

	return true;
}


Buffer::~Buffer()
{
	if (mContext && mBuffer != VK_NULL_HANDLE)
	{
		vmaDestroyBuffer(mContext->getVmaAllocator(), mBuffer, mAllocation);
	}
}


VkBuffer
Buffer::getVkBuffer()
{
	return mBuffer;
}


size_t Buffer::size() const
{
	return mSize;
}

Coral::BufferType
Buffer::type() const
{
	return mType;
}


std::byte*
Buffer::map()
{
	if (!mCpuVisible)
	{
		// TODO: Log error
		return nullptr;
	}

	void* data{ nullptr };

	if (vmaMapMemory(mContext->getVmaAllocator(), mAllocation, &data) != VK_SUCCESS)
	{
		// TODO: Log error
		return nullptr;
	}

	mMapped = (std::byte*)data;
	return mMapped;
}


bool
Buffer::unmap()
{
	if (!mMapped)
	{
		// TODO: Log error
		return false;
	}

	mMapped = nullptr;
	vmaUnmapMemory(mContext->getVmaAllocator(), mAllocation);

	return true;
}
