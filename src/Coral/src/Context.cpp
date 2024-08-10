module;

#include <cassert>

module Coral.Context;

import Coral.Factory;
import Coral.Vulkan.Context;


Coral::Context*
Coral::createContext(const Coral::ContextConfig& config)
{
    switch (config.graphicsAPI)
    {
        case Coral::GraphicsAPI::VULKAN:
        {
            return Coral::Factory::create<Vulkan::Context>(config);
        }
    }

    assert(false);
    return nullptr;
}


void
Coral::destroy(Coral::Context* context)
{
    Coral::Factory::destroy(context);
}
