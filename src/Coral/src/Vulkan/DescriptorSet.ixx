module;

#include <VulkanHeader.h>

export module Coral.Vulkan.DescriptorSet;

import Coral.DescriptorSet;

export namespace Coral
{

export namespace Vulkan
{

class Context;

class DescriptorSet: public Coral::DescriptorSet
{
public:

	virtual ~DescriptorSet();

	bool init(Context* context, const Coral::DescriptorSetConfig& config);

	VkDescriptorSet getVkDescriptorSet();

private:

	Context* mContext{ nullptr };

	VkDescriptorSetLayout mDescriptorSetLayout{ VK_NULL_HANDLE };

	VkDescriptorSet mDescriptorSet{ VK_NULL_HANDLE };
};

} // namespace Vulkan
	
} // namespace Coral
