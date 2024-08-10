module;

#include <cassert>

module Coral.Surface;

import Coral.Factory;
import Coral.Vulkan.Context;
import Coral.Vulkan.Surface;


Coral::Surface*
Coral::createSurface(Coral::Context* context, const Coral::SurfaceConfig& config)
{
    switch (context->graphicsAPI())
    {
        case Coral::GraphicsAPI::VULKAN:
        {
            return Coral::Factory::create<Vulkan::Surface>(static_cast<Coral::Vulkan::Context*>(context), config);
        }
    }

    assert(false);
    return nullptr;
}


void
Coral::destroy(Coral::Surface* surface)
{
    Coral::Factory::destroy(surface);
}
