module;

#include <cassert>

module Coral.Semaphore;

import Coral.Factory;
import Coral.Vulkan.Context;
import Coral.Vulkan.Semaphore;


Coral::Semaphore*
Coral::createSemaphore(Coral::Context* context)
{
    switch (context->graphicsAPI())
    {
        case Coral::GraphicsAPI::VULKAN:
        {
            return Coral::Factory::create<Coral::Vulkan::Semaphore>(static_cast<Coral::Vulkan::Context*>(context));
        }
    }

    assert(false);
    return nullptr;
}


void
Coral::destroy(Coral::Semaphore* semaphore)
{
    Coral::Factory::destroy(semaphore);
}
