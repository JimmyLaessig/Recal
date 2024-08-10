module;

#include <VulkanHeader.h>

export module Coral.Vulkan.Fence;

export import Coral.Fence;

namespace Coral
{

namespace Vulkan
{
export class Context;

export class Fence: public Coral::Fence
{
public:

	virtual ~Fence();

	bool init(Vulkan::Context* context);

	VkFence getVkFence();

	bool wait() override;

	void reset() override;

private:

	Context* mContext{ nullptr };

	VkFence mFence{ VK_NULL_HANDLE };

};

} // namespace Vulkan

} // namespace Coral
