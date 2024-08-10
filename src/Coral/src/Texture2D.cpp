module;

#include <cassert>

module RenderLib.Texture2D;

import RenderLib.Factory;
//import RenderLib.OpenGL.Context;
//import RenderLib.OpenGL.Texture2D;
import RenderLib.Types;

RenderLib::Texture2D*
RenderLib::createTexture2D(RenderLib::Context* context, const RenderLib::Texture2DConfig& config)
{
    //switch (context->graphicsAPI())
    //{
    //case RenderLib::GraphicsAPI::OPENGL:
    //{
    //    return RenderLib::Factory::create<OpenGL::Texture2DImpl>(static_cast<RenderLib::OpenGL::Context*>(context), config);
    //}
    //}

    assert(false);
    return nullptr;
}


void
RenderLib::destroy(RenderLib::Texture2D* Texture2D)
{
    RenderLib::Factory::destroy(Texture2D);
}
