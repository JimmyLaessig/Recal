module;

#include <cassert>

module Coral.PipelineState;

import Coral.Factory;
import Coral.Vulkan.Context;
import Coral.Vulkan.PipelineState;


Coral::PipelineState*
Coral::createPipelineState(Coral::Context* context, const Coral::PipelineStateConfig& config)
{
    switch (context->graphicsAPI())
    {
        case Coral::GraphicsAPI::VULKAN:
        {
            return Coral::Factory::create<Coral::Vulkan::PipelineState>(static_cast<Coral::Vulkan::Context*>(context), config);
        }
    }

    assert(false);
    return nullptr;
}


void
Coral::destroy(Coral::PipelineState* PipelineState)
{
    Coral::Factory::destroy(PipelineState);
}

using namespace Coral;

FaceCullingMode FaceCullingMode::BackFaceCulling = { CullMode::BACK, FrontFaceOrientation::CCW };

FaceCullingMode FaceCullingMode::FrontFaceCulling = { CullMode::FRONT, FrontFaceOrientation::CCW };

FaceCullingMode FaceCullingMode::None = { CullMode::NONE, FrontFaceOrientation::CCW };


BlendMode BlendMode::Blend = { BlendFactor::SRC_ALPHA, BlendFactor::ONE_MINUS_SRC_ALPHA, BlendOp::ADD };

BlendMode BlendMode::Additive = { BlendFactor::ONE, BlendFactor::ONE, BlendOp::ADD };

BlendMode BlendMode::None = { BlendFactor::ONE, BlendFactor::ZERO, BlendOp::ADD };


DepthTestMode DepthTestMode::None = { true, Coral::CompareOp::ALWAYS, {} };

DepthTestMode DepthTestMode::Less = { true, Coral::CompareOp::LESS, {} };

DepthTestMode DepthTestMode::LessOrEqual = { true, Coral::CompareOp::LESS_OR_EQUAL, {} };
