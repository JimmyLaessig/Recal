module;

#include <VulkanHeader.h>

#include <map>
#include <memory>
#include <mutex>
#include <span>

export module Coral.Vulkan.Context;

export import Coral.Context;

import Coral.Vulkan.BufferPool;

namespace Coral
{

namespace Vulkan
{
// Forward declaration
export class Buffer;
export class CommandQueue;
export class Semaphore;

export class Context : public Coral::Context
{
public:

	bool init(const ContextConfig& config);

	virtual ~Context();

	GraphicsAPI graphicsAPI() const override { return GraphicsAPI::VULKAN; }

	Coral::CommandQueue* getGraphicsQueue() override;

	Coral::CommandQueue* getComputeQueue() override;
									
	Coral::CommandQueue* getTransferQueue() override;

	VkInstance getVkInstance() { return mInstance; }

	VkDevice getVkDevice() { return mDevice; }

	VkPhysicalDevice getVkPhysicalDevice() { return mPhysicalDevice; }

	VmaAllocator getVmaAllocator();

	VkDescriptorPool getVkDescriptorPool();

	uint32_t getQueueFamilyIndex();

	/// Request a staging buffer from the staging buffer pool
	/**
	 * The staging buffer will have at least the requested buffer size. Staging buffers should be returned to the pool
	 * after the using command buffer was submitted.
	 */
	std::unique_ptr<Coral::Vulkan::Buffer> requestStagingBuffer(size_t bufferSize);

	/// Return the staging buffers to the command pool
	/**
	 * The staging buffers should be returned to the pool once the using command buffer was submitted. The
	 * waitSemaphore ensures that the buffers are only readded to the pool, once execution of the command buffer has
	 * finished. Note, that for that, the waitSemaphore must be part of the signalSemaphores of the SubmitInfo that
	 * submits the command buffer to the command queue.
	 * buffers should be returned to the pool after the using command buffer was submitted.
	 */
	void returnStagingBuffersAfterUse(std::vector<std::unique_ptr<Coral::Vulkan::Buffer>>&& stagingBuffers,
									  std::unique_ptr<Coral::Vulkan::Semaphore>&& waitSemaphore);

private:

	VkInstance mInstance{ VK_NULL_HANDLE };

	VkDebugUtilsMessengerEXT mDebugMessenger{ VK_NULL_HANDLE };

	VkPhysicalDevice mPhysicalDevice{ VK_NULL_HANDLE };

	VkDevice mDevice{ VK_NULL_HANDLE };

	VmaAllocator mAllocator{ VK_NULL_HANDLE };

	VkCommandPool mCommandPool{ VK_NULL_HANDLE };

	VkDescriptorPool mDescriptorPool{ VK_NULL_HANDLE };

	uint32_t mQueueFamilyIndex{ 0 };

	std::shared_ptr<CommandQueue> mTransferQueue;

	std::shared_ptr<CommandQueue> mGraphicsQueue;

	std::shared_ptr<CommandQueue> mComputeQueue;

	std::mutex mCommandPoolsProtection;

	std::unique_ptr<BufferPool> mStagingBufferPool;

}; // class Context

} // namespace Vulkan

} // namespace Coral