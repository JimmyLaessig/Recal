module;

#include <cassert>

module RenderLib.UniformBuffer;

//import RenderLib.Factory;
//import RenderLib.OpenGL.Context;
//import RenderLib.OpenGL.UniformBuffer;
import RenderLib.Types;

RenderLib::UniformBuffer*
RenderLib::createUniformBuffer(RenderLib::Context* context, const RenderLib::UniformBufferConfig& config)
{
    //switch (context->graphicsAPI())
    //{
    //case RenderLib::GraphicsAPI::OPENGL:
    //{
    //    return RenderLib::Factory::create<OpenGL::UniformBufferImpl>(static_cast<RenderLib::OpenGL::Context*>(context), config);
    //}
    //}

    assert(false);
    return nullptr;
}


void
RenderLib::destroy(RenderLib::UniformBuffer* uniformBuffer)
{
    //RenderLib::Factory::destroy(uniformBuffer);
}
