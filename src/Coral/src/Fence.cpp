module;

#include <cassert>

module Coral.Fence;

import Coral.Factory;
import Coral.Vulkan.Context;
import Coral.Vulkan.Fence;


Coral::Fence*
Coral::createFence(Coral::Context* context)
{
    switch (context->graphicsAPI())
    {
        case Coral::GraphicsAPI::VULKAN:
        {
            return Coral::Factory::create<Coral::Vulkan::Fence>(static_cast<Coral::Vulkan::Context*>(context));
        }
    }

    assert(false);
    return nullptr;
}


void
Coral::destroy(Coral::Fence* fence)
{
    Coral::Factory::destroy(fence);
}
