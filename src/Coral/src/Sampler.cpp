module;

#include <cassert>

module Coral.Sampler;

import Coral.Factory;
import Coral.Vulkan.Context;
import Coral.Vulkan.Sampler;


Coral::Sampler*
Coral::createSampler(Coral::Context* context, const Coral::SamplerConfig& config)
{
    switch (context->graphicsAPI())
    {
        case Coral::GraphicsAPI::VULKAN:
        {
            return Coral::Factory::create<Coral::Vulkan::Sampler>(static_cast<Coral::Vulkan::Context*>(context), config);
        }
    }

    assert(false);
    return nullptr;
}


void
Coral::destroy(Coral::Sampler* sampler)
{
    Coral::Factory::destroy(sampler);
}
