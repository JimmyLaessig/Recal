module Engine.Rendering.Renderer;

import Engine.Rendering.ShaderManager;
import Engine.Rendering.DefaultShaders;

using namespace Reef::Rendering;


bool
Renderer::initialize(RenderLib::Context& context)
{
	mCommandQueue	= RenderLib::RAII::createCommandQueue(&context, {});
	mContext		= &context;
	mIsInitialized	= true;

	return mResourceManager.initialize(context) &&
		   mShaderManager.initialize(context) &&
		   Engine::Rendering::DefaultShaders::initialize(mShaderManager);
}


bool
Renderer::isInitialized()
{
	return mIsInitialized;
}


void
Renderer::submitCommandBuffer(RenderLib::CommandBuffer* commandBuffer)
{
	mCommandQueue->submit(commandBuffer);
}


void
Renderer::execute()
{
	mCommandQueue->execute();
}


RenderLib::Context&
Renderer::context()
{
	return *mContext;
}


ResourceManager&
Renderer::resourceManager()
{
	return mResourceManager;
}


ShaderManager&
Renderer::shaderManager()
{
	return mShaderManager;
}