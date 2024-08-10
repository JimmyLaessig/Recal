module;

#include <cassert>

module RenderLib.Texture3D;

import RenderLib.Factory;
//import RenderLib.OpenGL.Context;
//import RenderLib.OpenGL.Texture3D;
import RenderLib.Types;

RenderLib::Texture3D*
RenderLib::createTexture3D(RenderLib::Context* context, const RenderLib::Texture3DConfig& config)
{
    //switch (context->graphicsAPI())
    //{
    //case RenderLib::GraphicsAPI::OPENGL:
    //{
    //    return RenderLib::Factory::create<OpenGL::Texture3DImpl>(static_cast<RenderLib::OpenGL::Context*>(context), config);
    //}
    //}

    assert(false);
    return nullptr;
}


void
RenderLib::destroy(RenderLib::Texture3D* Texture3D)
{
    RenderLib::Factory::destroy(Texture3D);
}
