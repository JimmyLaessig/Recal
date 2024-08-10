module;

#include <Coral/System.h>

#include <cstdint>
#include <optional>
#include <span>
#include <string_view>

export module Coral.PipelineState;

export import Coral.Types;

import Coral.Framebuffer;

export namespace Coral
{
class ShaderModule;
class Context;

///
enum class CompareOp
{
	NEVER,
	LESS,
	EQUAL,
	LESS_OR_EQUAL,
	GREATER,
	GREATER_OR_EQUAL,
	NOT_EQUAL,
	ALWAYS
};


/// Cull Mode
enum class CullMode
{
	FRONT,
	BACK,
	FRONT_AND_BACK,
	NONE,
};


/// Front Face Orientation
enum class FrontFaceOrientation
{
	CW,
	CCW
};


/// Face Culling Mode
struct FaceCullingMode
{
	CullMode cullMode{ CullMode::BACK };

	FrontFaceOrientation orientation{ FrontFaceOrientation::CCW };

	static FaceCullingMode BackFaceCulling;

	static FaceCullingMode FrontFaceCulling;

	static FaceCullingMode None;
};


/// Blend Factor
enum class BlendFactor
{
	ZERO,
	ONE,
	SRC_ALPHA,
	ONE_MINUS_SRC_ALPHA,
	DST_ALPHA,
	ONE_MINUS_DST_ALPHA
};


/// Blend Op
enum class BlendOp
{
	ADD,
	SUBTRACT,
	REVERSE_SUBTRACT,
	MIN,
	MAX
};


struct BlendMode
{
	BlendFactor srcFactor{ BlendFactor::ONE };
	BlendFactor destFactor{ BlendFactor::ONE };
	BlendOp blendOp{ BlendOp::ADD };

	static BlendMode Blend;

	static BlendMode Additive;

	static BlendMode None;
};


//BlendMode BlendModeOpaque = { BlendFactor::ONE, BlendFactor::ZERO, BlendOp::ADD };
//
//BlendMode BlendModeTransparent = { BlendFactor::SRC_ALPHA, BlendFactor::ONE_MINUS_SRC_ALPHA, BlendOp::ADD };
//
//BlendMode BlendModeAdd = { BlendFactor::ONE, BlendFactor::ONE, BlendOp::ADD };


// -----------------------------------------
//
//    Stencil Test
//
// -----------------------------------------

/// Stencil Op
enum class StencilOp
{
	KEEP,
	ZERO,
	REPLACE,
	INCREMENT,
	INCREMENT_WRAP,
	DECREMENT,
	DECREMENT_WRAP,
	INVERT
};


/// Stencil Op State
struct StencilOpState
{
	StencilOp failOp		{ StencilOp::KEEP	};
	StencilOp depthFailOp	{ StencilOp::KEEP	};
	StencilOp depthPassOp	{ StencilOp::KEEP	};
	CompareOp func			{ CompareOp::ALWAYS	};
	uint32_t ref			{ 0					};
	uint32_t mask			{ 0xFFFFFFFF		};
};


/// StencilTestMode
struct StencilTestMode
{
	StencilOpState front;
	StencilOpState back;
};


/// Polygon offset
struct PolygonOffset
{
	float factor{ 0.0f };
	float units { 0.0f };
};


/// Depth test mode
struct DepthTestMode
{
	bool writeDepth { true };

	CompareOp compareOp	{ CompareOp::LESS };

	PolygonOffset polygonOffset;

	static DepthTestMode None;

	static DepthTestMode Less;

	static DepthTestMode LessOrEqual;
};


enum class PolygonMode
{
	SOLID,
	WIREFRAME,
	POINTS
};


struct RasterizerMode
{

};


enum class Topology
{
	POINT_LIST,
	TRIANGLE_LIST,
	LINE_LIST,
};


///
struct PipelineStateConfig
{
	std::span<ShaderModule*> shaderModules;

	FramebufferSignature framebufferSignature;

	/// The face culling mode of the pipeline 
	/**
	 * Default is back-face culling (front faces are CCW)
	 */
	FaceCullingMode faceCullingMode = FaceCullingMode::BackFaceCulling;

	/// The depth test mode of the pipeline. 
	/**
	 * Default is depth test enabled with LESS_OR_EQUAL compare op.
	 */
	DepthTestMode depthTestMode = DepthTestMode::LessOrEqual;

	/// The stencil test mode of the pipeline. 
	/**
	 * If unset, no stencil test is performed.
	 */
	//StencilTestMode* stencilTestMode{ nullptr };

	/// The blend mode of the pipeline. 
	/**
	 * Default is no blending
	 */
	BlendMode blendMode = BlendMode::None;

	/// The polygon mode of the pipeline
	/**
	 * Default is SOLID.
	 */
	PolygonMode polygonMode = PolygonMode::SOLID;

	Topology topology{ Topology::TRIANGLE_LIST };
};


class CORAL_API PipelineState
{
public:

	virtual ~PipelineState() = default;

};


CORAL_API PipelineState* createPipelineState(Context* context, const PipelineStateConfig& config);

CORAL_API void destroy(PipelineState* pipelineState);

} // namespace Coral