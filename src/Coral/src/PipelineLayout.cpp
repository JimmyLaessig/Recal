module;

#include <cassert>

module Coral.PipelineLayout;

import Coral.Factory;

import Coral.Types;

import Coral.Vulkan.Context;
import Coral.Vulkan.PipelineLayout;

Coral::PipelineLayout*
Coral::createPipelineLayout(Coral::Context* context, const Coral::PipelineLayoutConfig& config)
{
    switch (context->graphicsAPI())
    {
        case Coral::GraphicsAPI::VULKAN:
        {
            return Coral::Factory::create<Coral::Vulkan::PipelineLayout>(static_cast<Coral::Vulkan::Context*>(context), config);
        }
    }

    assert(false);
    return nullptr;
}


void
Coral::destroy(Coral::PipelineLayout* PipelineLayout)
{
    Coral::Factory::destroy(PipelineLayout);
}
