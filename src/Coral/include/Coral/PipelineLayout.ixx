module;

#include <Coral/System.h>

#include <cstdint>
#include <optional>
#include <span>

export module Coral.PipelineLayout;

export import Coral.Context;
export import Coral.Types;

export namespace Coral
{

struct AttributeDescription
{
	/// The shader location to which to bind the attribute
	uint32_t location{ 0 };

	// The format of the attribute
	AttributeFormat format{ AttributeFormat::VEC3 };

	uint32_t offset{ 0 };

	uint32_t stride{ 0 };
};


struct AttachmentDescription
{
	uint32_t location{ 0 };

	PixelFormat format{ PixelFormat::RGBA8_SRGB };
};




///
struct PipelineLayoutConfig
{
	std::span<AttributeDescription> vertexInputAttributeDescriptions;

	std::span<AttachmentDescription> colorAttachmentDescription;

	std::optional<AttachmentDescription> depthStencilAttachmentDescription;
};


class CORAL_API PipelineLayout
{
public:

};


CORAL_API PipelineLayout* createPipelineLayout(Context* context, const PipelineLayoutConfig& config);

CORAL_API void destroy(PipelineLayout* pipelineState);

} // namespace Coral