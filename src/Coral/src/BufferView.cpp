module;

#include <cassert>

module Coral.BufferView;

import Coral.Factory;
import Coral.Vulkan.BufferView;
import Coral.Vulkan.Context;


Coral::BufferView*
Coral::createBufferView(Coral::Context* context, const Coral::BufferViewConfig& config)
{
    switch (context->graphicsAPI())
    {
        case Coral::GraphicsAPI::VULKAN:
        {
            return Coral::Factory::create<Coral::Vulkan::BufferView>(static_cast<Coral::Vulkan::Context*>(context), config);
        }
    }

    assert(false);
    return nullptr;
}


void
Coral::destroy(Coral::BufferView* bufferView)
{
    Coral::Factory::destroy(bufferView);
}
