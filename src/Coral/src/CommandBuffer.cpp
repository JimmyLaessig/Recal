module;

#include <cassert>

module Coral.CommandBuffer;

import Coral.Factory;
import Coral.Vulkan.CommandBuffer;
import Coral.Vulkan.Context;


Coral::CommandBuffer*
Coral::createCommandBuffer(Coral::Context* context, const Coral::CommandBufferConfig& config)
{
    switch (context->graphicsAPI())
    {
        case Coral::GraphicsAPI::VULKAN:
        {
            return Coral::Factory::create<Vulkan::CommandBuffer>(static_cast<Coral::Vulkan::Context*>(context), config);
        }
    }

    assert(false);
    return nullptr;
}


void
Coral::destroy(Coral::CommandBuffer* commandBuffer)
{
    Coral::Factory::destroy(commandBuffer);
}