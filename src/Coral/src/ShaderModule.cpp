module;

#include <cassert>

module Coral.ShaderModule;

import Coral.Factory;
import Coral.Vulkan.Context;
import Coral.Vulkan.ShaderModule;


Coral::ShaderModule*
Coral::createShaderModule(Coral::Context* context, const Coral::ShaderModuleConfig& config)
{
    switch (context->graphicsAPI())
    {
        case Coral::GraphicsAPI::VULKAN:
        {
            return Coral::Factory::create<Coral::Vulkan::ShaderModule>(static_cast<Coral::Vulkan::Context*>(context), config);
        }
    }

    assert(false);
    return nullptr;
}


void
Coral::destroy(Coral::ShaderModule* shaderModule)
{
    Coral::Factory::destroy(shaderModule);
}
