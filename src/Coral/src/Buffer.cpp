module;

#include <cassert>

module Coral.Buffer;

import Coral.Factory;
import Coral.Vulkan.Buffer;
import Coral.Vulkan.Context;


Coral::Buffer*
Coral::createBuffer(Coral::Context* context, const Coral::BufferConfig& config)
{
    switch (context->graphicsAPI())
    {
        case Coral::GraphicsAPI::VULKAN:
        {
            return Coral::Factory::create<Coral::Vulkan::Buffer>(static_cast<Coral::Vulkan::Context*>(context), config);
        }
    }

    assert(false);
    return nullptr;
}


void
Coral::destroy(Coral::Buffer* Buffer)
{
    Coral::Factory::destroy(Buffer);
}
