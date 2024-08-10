module;

#include <VulkanHeader.h>

#include <map>
#include <memory>
#include <mutex>
#include <span>
#include <vector>

export module Coral.Vulkan.BufferPool;

import Common.AsyncTaskQueue;

import Coral.Vulkan.Buffer;

export namespace Coral
{

export namespace Vulkan
{

class Context;
class Semaphore;

class BufferPool
{
public:

	BufferPool(Coral::Vulkan::Context& context, Coral::BufferType bufferType, bool cpuVisible);

	/// Request a buffer from the pool
	/**
	 * The buffer will have at least the requested buffer size. Staging buffers should be returned to the pool
	 * after the using command buffer was submitted.
	 */
	std::unique_ptr<Coral::Vulkan::Buffer> requestBuffer(size_t bufferSize);

	/// Return the buffers to the buffer pool
	/**
	 * The buffers should be returned to the pool once the using command buffer was submitted. The
	 * waitSemaphore ensures that the buffers are only readded to the pool, once execution of the command buffer has
	 * finished. Note, that for that, the waitSemaphore must be part of the signalSemaphores of the SubmitInfo that
	 * submits the command buffer to the command queue.
	 * buffers should be returned to the pool after the using command buffer was submitted.
	 */
	void returnBuffersAfterUse(std::vector<std::unique_ptr<Coral::Vulkan::Buffer>>&& buffers,
							   std::unique_ptr<Coral::Vulkan::Semaphore>&& waitSemaphore);

private:

	Coral::Vulkan::Context* mContext{ nullptr };

	std::multimap<size_t, std::unique_ptr<Coral::Vulkan::Buffer>> mBufferPool;

	std::mutex mBufferPoolProtection;

	Common::AsyncTaskQueue mBufferReturnQueue;

	Coral::BufferType mBufferType{ Coral::BufferType::STORAGE_BUFFER };

	bool mCpuVisible{ false };
}; // class Context

} // namespace Vulkan

} // namespace Coral