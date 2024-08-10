module;

#include <VulkanHeader.h>

#include <mutex>
#include <thread>
#include <unordered_map>

export module Coral.Vulkan.CommandQueue;

export import Coral.CommandQueue;


namespace Coral
{

namespace Vulkan
{

export class Context;

export class CommandQueue : public Coral::CommandQueue
{
public:
	CommandQueue() = default;

	CommandQueue(Context* context, VkQueue queue, uint32_t index, uint32_t mQueueFamilyIndex);

	virtual ~CommandQueue();

	bool submit(const Coral::CommandBufferSubmitInfo& info, Fence* fence) override;

	bool submit(const Coral::PresentInfo& info) override;

	bool waitIdle() override;

	uint32_t getQueueIndex()
	{
		return mQueueIndex;
	}

	VkCommandPool getVkCommandPool();

	VkQueue getVkQueue();

private:

	Coral::Vulkan::Context* mContext{ nullptr };

	std::mutex mQueueProtection;

	VkQueue mQueue{ VK_NULL_HANDLE };

	uint32_t mQueueIndex{ 0 };

	uint32_t mQueueFamilyIndex{ 0 };

	std::unordered_map<std::thread::id, VkCommandPool> mCommandPools;
}; // class CommandQueueImpl

} // namespace Vulkan

} // namespace Coral