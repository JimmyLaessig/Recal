module;

#include <Rendering/Shaders/DefaultPBR.h>

#include <array>
#include <memory>
#include <ranges>
#include <vector>

module Engine.Rendering.DefaultRenderer;

import Engine.Shader;

import Engine.Application;
import Engine.Application.Window;

import Coral.RAII;

using namespace Reef;

template<typename T, size_t S>
std::span<std::byte> ByteSpan(const std::array<T, S>& a)
{
	return { (std::byte*)a.data(), a.size() * sizeof(T) };
}


bool
DefaultRenderer::initializeDefaultShaders()
{
	auto& shaderManager = mApplication->shaderManager();

	//auto x = vertexShader->bindingDescriptions();
	//auto y = vertexShader->bindingDescriptions();

	auto defaultPBR = shaderManager.createShader(Reef::BuiltInShaders::DefaultPBR, 
												 ByteSpan(Shaders::DefaultPBR::vertexShaderSourceSpirV), 
												 ByteSpan(Shaders::DefaultPBR::fragmentShaderSourceSpirV));

	if (!defaultPBR)
	{
		return false;
	}
	return true;
}


bool
DefaultRenderer::initialize(IApplication& application)
{
	mApplication = &application;

	auto window = mApplication->window();

	Coral::SurfaceConfig surfaceConfig{};
	surfaceConfig.nativeWindowHandle					= window->nativeWindowHandle();
	surfaceConfig.swapchainConfig.depthFormat			= Coral::PixelFormat::DEPTH24_STENCIL8;
	surfaceConfig.swapchainConfig.format				= Coral::PixelFormat::RGBA8_SRGB;
	surfaceConfig.swapchainConfig.swapchainImageCount	= 2;

	Coral::ContextConfig contextConfig{};
	contextConfig.graphicsAPI	= Coral::GraphicsAPI::VULKAN;

	mContext = Coral::RAII::createContext(contextConfig);

	if (!mContext)
	{
		return false;
	}

	mSurface = Coral::RAII::createSurface(mContext.get(), surfaceConfig);

	if (!mSurface)
	{
		return false;
	}

	return initializeDefaultShaders();
}


void
DefaultRenderer::render()
{

}