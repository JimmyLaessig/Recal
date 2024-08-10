module;

#include <VulkanHeader.h>

#include <optional>
#include <span>
#include <vector>

export module Coral.Vulkan.PipelineState;

export import Coral.PipelineState;


namespace Coral
{
namespace Vulkan
{
export class Context;

export class PipelineState : public Coral::PipelineState
{
public:

	virtual ~PipelineState();

	bool init(Coral::Vulkan::Context* context, const PipelineStateConfig& config);

	VkPipeline getVkPipeline();

	std::span<VkDescriptorSetLayout> getVkDescriptorSetLayouts();

	VkPipelineLayout getVkPipelineLayout();

private:

	Context* mContext{ nullptr };

	VkPipelineLayout mPipelineLayout{ VK_NULL_HANDLE };

	VkPipeline mPipeline{ VK_NULL_HANDLE };

	std::vector<VkDescriptorSetLayout> mDescriptorSetLayouts;

	FaceCullingMode mFaceCullingMode;

	DepthTestMode mDepthTestMode;

	BlendMode mBlendMode;

	PolygonMode mPolygonMode;

	std::optional<StencilTestMode> mStencilTestMode;
};

} // namespace Vulkan

} // namespace Coral