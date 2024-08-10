module;

#include <cassert>

module Coral.DescriptorSet;

import Coral.Factory;
import Coral.Vulkan.Context;
import Coral.Vulkan.DescriptorSet;


Coral::DescriptorSet*
Coral::createDescriptorSet(Coral::Context* context, const Coral::DescriptorSetConfig& config)
{
    switch (context->graphicsAPI())
    {
        case Coral::GraphicsAPI::VULKAN:
        {
            return Coral::Factory::create<Coral::Vulkan::DescriptorSet>(static_cast<Coral::Vulkan::Context*>(context), config);
        }
    }

    assert(false);
    return nullptr;
}


void
Coral::destroy(Coral::DescriptorSet* descriptorSet)
{
    Coral::Factory::destroy(descriptorSet);
}
