module;

#include <string>
#include <vector>
#include <optional>
#include <memory>
#include <span>

#include <vulkan/vulkan.h>

export module Coral.Vulkan.PipelineLayout;

export import Coral.PipelineLayout;

import Coral.Vulkan.Context;

namespace Coral
{
namespace Vulkan
{

export class PipelineLayout : public Coral::PipelineLayout
{
public:

	~PipelineLayout();

	bool init(Context* context, const PipelineLayoutConfig& config);

	std::span<VkDescriptorSetLayout> getVkDescriptorSetLayouts();

	VkPipelineLayout getVkPipelineLayout();
private:

	Context* mContext{ nullptr };

	VkPipelineLayout mPipelineLayout{ VK_NULL_HANDLE };

	std::vector<VkDescriptorSetLayout> mDescriptorSetLayouts;
};

} // namespace Vulkan

} // namespace Coral