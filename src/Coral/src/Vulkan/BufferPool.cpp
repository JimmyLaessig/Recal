module;

#include <memory>
#include <mutex>

module Coral.Vulkan.BufferPool;

import Coral.Buffer;
import Coral.Context;

import Coral.Vulkan.Context;
import Coral.Vulkan.CommandQueue;
import Coral.Vulkan.Surface;
import Coral.Vulkan.SurfaceWin32;

import Coral.Vulkan.Semaphore;

//import Log20;

using namespace Coral::Vulkan;


BufferPool::BufferPool(Coral::Vulkan::Context& context, Coral::BufferType bufferType, bool cpuVisible)
{
	mContext = &context;
	mBufferType = bufferType;
	mCpuVisible = cpuVisible;
}


std::unique_ptr<Coral::Vulkan::Buffer>
BufferPool::requestBuffer(size_t bufferSize)
{
	if (!mContext)
	{
		return nullptr;
	}

	std::lock_guard lock(mBufferPoolProtection);

	// Find the smallest staging buffer that fits the buffer size
	auto candidate = mBufferPool.end();
	for (auto iter = mBufferPool.begin(); iter != mBufferPool.end(); iter++)
	{
		if (iter->first >= bufferSize)
		{
			candidate = iter;
		}
	}

	// If no staging buffer was found, create one that fits the buffer size
	if (candidate != mBufferPool.end())
	{
		auto buffer = std::move(candidate->second);
		mBufferPool.erase(candidate);
	}

	Coral::BufferConfig bufferConfig{};
	bufferConfig.cpuVisible = mCpuVisible;
	bufferConfig.type		= mBufferType;
	bufferConfig.size		= bufferSize;

	return std::unique_ptr<Coral::Vulkan::Buffer>(static_cast<Coral::Vulkan::Buffer*>(Coral::createBuffer(mContext, bufferConfig)));
}


void
BufferPool::returnBuffersAfterUse(std::vector<std::unique_ptr<Coral::Vulkan::Buffer>>&& stagingBuffers,
								  std::unique_ptr<Coral::Vulkan::Semaphore>&& waitSemaphore)
{
	if (!mContext)
	{
		return;
	}

	// unique_ptr can't be stored in a std::function directly. Therefore, we need to wrap the data into a shared_ptr
	// and pass it to the lambda that way.

	struct Data
	{
		std::vector<std::unique_ptr<Coral::Vulkan::Buffer>> stagingBuffers;
		std::unique_ptr<Coral::Vulkan::Semaphore> waitSemaphore;
	};

	auto data = std::make_shared<Data>();

	data->stagingBuffers = std::move(stagingBuffers);
	data->waitSemaphore = std::move(waitSemaphore);

	auto task = [this, data = std::move(data)]
	{
		if (data->waitSemaphore->wait())
		{
			std::lock_guard lock(mBufferPoolProtection);
			for (auto& buffer : data->stagingBuffers)
			{
				this->mBufferPool.insert({ buffer->size(), std::move(buffer) });
			}
		}
	};

	mBufferReturnQueue.enqueue(std::move(task));
}
