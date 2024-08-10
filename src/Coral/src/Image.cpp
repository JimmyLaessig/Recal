module;

#include <cassert>

module Coral.Image;

import Coral.Factory;
import Coral.Vulkan.Context;
import Coral.Vulkan.Image;


Coral::Image*
Coral::createImage(Coral::Context* context, const Coral::ImageConfig& config)
{
    switch (context->graphicsAPI())
    {
        case Coral::GraphicsAPI::VULKAN:
        {
            return Coral::Factory::create<Coral::Vulkan::Image>(static_cast<Coral::Vulkan::Context*>(context), config);
        }
    }

    assert(false);
    return nullptr;
}


void
Coral::destroy(Coral::Image* image)
{
    Coral::Factory::destroy(image);
}
