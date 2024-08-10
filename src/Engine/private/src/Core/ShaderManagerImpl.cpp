module;

#include <chrono>
#include <mutex>
#include <string>
#include <string_view>
#include <vector>
#include <memory>
#include <unordered_map>

module Engine.ShaderManagerImpl;

import Engine.Application.ApplicationImpl;
import Coral.RAII;

using namespace Reef;

ShaderManagerImpl::ShaderManagerImpl(ApplicationImpl& application)
	: mApplication(&application)
{}


Shader*
ShaderManagerImpl::getShader(std::string_view name)
{
	std::string key{ name.data(), name.size() };
	
	auto iter = mShaders.find(key);

	if (iter == mShaders.end())
	{
		return nullptr;
	}

	return iter->second.get();
}


Shader* 
ShaderManagerImpl::createShader(std::string_view name,
								std::span<const std::byte> vertexShaderSource,
								std::span<const std::byte> fragmentShaderSource)
{
	auto context = mApplication->renderContext();

	if (!context)
	{
		return nullptr;
	}

	std::string key{ name.data(), name.size() };

	if (mShaders.contains(key))
	{
		return nullptr;
	}

	std::string vertexShaderName = std::string(name) + "_VertexShader";
	Coral::ShaderModuleConfig vertexShaderConfig{};
	vertexShaderConfig.name = vertexShaderName;
	vertexShaderConfig.stage = Coral::ShaderStage::VERTEX;
	vertexShaderConfig.source = vertexShaderSource;
	auto vertexShader = Coral::RAII::createShaderModule(context, vertexShaderConfig);

	if (!vertexShader)
	{
		return nullptr;
	}

	std::string fragmentShaderName = std::string(name) + "_FragmentShader";
	Coral::ShaderModuleConfig fragmentShaderConfig{};
	fragmentShaderConfig.name = fragmentShaderName;
	fragmentShaderConfig.stage = Coral::ShaderStage::FRAGMENT;
	fragmentShaderConfig.source = fragmentShaderSource;
	auto fragmentShader = Coral::RAII::createShaderModule(context, fragmentShaderConfig);

	if (!fragmentShader)
	{
		return nullptr;
	}

	return nullptr;
}
