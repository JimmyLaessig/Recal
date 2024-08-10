module;

#include <VulkanHeader.h>

#include <optional>
#include <vector>

export module Coral.Vulkan.Framebuffer;

export import Coral.Framebuffer;

import Coral.PipelineState;

namespace Coral
{

namespace Vulkan
{

export class Context;

export class Framebuffer : public Coral::Framebuffer
{
public:

	bool init(Coral::Vulkan::Context* context, const Coral::FramebufferConfig& config);

	const std::vector<ColorAttachment>& colorAttachments();

	const std::optional<DepthAttachment>& depthAttachment();

	uint32_t width();

	uint32_t height();

	Coral::FramebufferSignature getSignature() override;

private:

	Coral::Vulkan::Context* mContext{ nullptr };

	std::vector<ColorAttachment> mColorAttachments;

	std::optional<DepthAttachment> mDepthAttachment;

	uint32_t mWidth{ 0 };

	uint32_t mHeight{ 0 };

}; // class Framebuffer

} // namespace Vulkan

} // namespace Coral