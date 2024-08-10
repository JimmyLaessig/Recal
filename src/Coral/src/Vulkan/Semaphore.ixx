module;

#include <VulkanHeader.h>

#include <limits>

export module Coral.Vulkan.Semaphore;

export import Coral.Semaphore;

namespace Coral
{

namespace Vulkan
{

export class Context;

export class Semaphore : public Coral::Semaphore
{
public:

	bool init(Coral::Vulkan::Context* context);

	virtual ~Semaphore();

	VkSemaphore getVkSemaphore();

	bool wait(uint64_t timeout = std::numeric_limits<uint64_t>::max());

private:

	Coral::Vulkan::Context* mContext{ nullptr };

	VkSemaphore mSemaphore{ VK_NULL_HANDLE };

}; // class Semaphore

} // namespace Vulkan

} // namespace Coral