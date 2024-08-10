module;

#include <cassert>

module Coral.Framebuffer;

import Coral.Factory;
import Coral.Vulkan.Context;
import Coral.Vulkan.Framebuffer;


Coral::Framebuffer*
Coral::createFramebuffer(Coral::Context* context, const Coral::FramebufferConfig& config)
{
    switch (context->graphicsAPI())
    {
        case Coral::GraphicsAPI::VULKAN:
        {
            return Coral::Factory::create<Coral::Vulkan::Framebuffer>(static_cast<Coral::Vulkan::Context*>(context), config);
        }
    }

    assert(false);
    return nullptr;
}


void
Coral::destroy(Coral::Framebuffer* framebuffer)
{
    Coral::Factory::destroy(framebuffer);
}
