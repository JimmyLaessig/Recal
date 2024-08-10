module;

#include "Rendering/Shaders/generated/CalculateIrradianceShader.h"
#include "Rendering/Shaders/generated/DefaultPBR.h"
#include "Rendering/Shaders/generated/DirectionalLightShader.h"
#include "Rendering/Shaders/generated/UnlitSkyboxShader.h"
#include "Rendering/Shaders/generated/EnvironmentLightShader.h"
#include "Rendering/Shaders/generated/SSAO.h"
#include "Rendering/Shaders/generated/AverageBlurShader.h"
#include "Rendering/Shaders/generated/DepthShader.h"
#include "Rendering/Shaders/generated/ToneMappingShader.h"
#include "Rendering/Shaders/generated/GammaCorrectionShader.h"
#include "Rendering/Shaders/generated/FXAAShader.h"
#include "Rendering/Shaders/generated/UniformColor.h"
#include "Rendering/Shaders/generated/Selection.h"

module Engine.Rendering.ShaderManager;
import Engine.Rendering.ShaderResource;

import RenderLib.Context;
import RenderLib.Shader;

import <memory>;
import <string_view>;
import <unordered_map>;

using namespace Reef::Rendering;

RenderLib::Context* ShaderManager::sContext = nullptr;

std::unordered_map<size_t, std::shared_ptr<Engine::Shader>> ShaderManager::sShaders;

std::shared_ptr<Engine::Shader>
ShaderManager::addShader(std::string_view name,
						  std::string_view vertexShaderSource,
						  std::string_view fragmentShaderSource)
{
	RenderLib::ShaderConfig config{};
	config.name					= name;
	config.vertexShaderSource	= vertexShaderSource;
	config.fragmentShaderSource = fragmentShaderSource;
	auto shader = RenderLib::RAII::createShader(sContext, config);

	if (!shader)
	{
		return nullptr;
	}

	auto shader = std::make_shared<Engine::Shader>(name, std::move(shader));

	ShaderManager::sShaders[std::hash<std::string_view>()(name)] = shader;

	return shader;
}


std::shared_ptr<Engine::Shader>
ShaderManager::find(std::string_view name)
{
	auto iter = ShaderManager::sShaders.find(std::hash<std::string_view>()(name));

	if (iter == sShaders.end())
	{
		return nullptr;
	}

	return iter->second;
}


bool
ShaderManager::initialize(RenderLib::Context& context)
{
	sContext = &context;

	auto addShaderHelper = [&](std::string_view name, std::string_view vertexShaderSource, std::string_view fragmentShaderSource)
	{

		return addShader(name, vertexShaderSource, fragmentShaderSource)->renderResource().update(context);
	};

	return 
		addShaderHelper("SelectionShader",
						Shaders::Selection::vertexShaderSourceGLSL,
						Shaders::Selection::fragmentShaderSourceGLSL) &&

		addShaderHelper("UniformColor",
						Shaders::UniformColor::vertexShaderSourceGLSL,
						Shaders::UniformColor::fragmentShaderSourceGLSL) &&

		addShaderHelper("DefaultPBR",
						Shaders::DefaultPBR::vertexShaderSourceGLSL,
						Shaders::DefaultPBR::fragmentShaderSourceGLSL) &&

		addShaderHelper("UnlitSkybox",
						Shaders::UnlitSkybox::vertexShaderSourceGLSL,
						Shaders::UnlitSkybox::fragmentShaderSourceGLSL) &&

		addShaderHelper("CalculateIrradiance",
						Shaders::CalculateIrradianceShader::vertexShaderSourceGLSL,
						Shaders::CalculateIrradianceShader::fragmentShaderSourceGLSL) &&

		addShaderHelper("DirectionalLight",
						Shaders::DirectionalLightPass::vertexShaderSourceGLSL,
						Shaders::DirectionalLightPass::fragmentShaderSourceGLSL) &&

		addShaderHelper("EnvironmentLight",
						Shaders::EnvironmentLightShader::vertexShaderSourceGLSL,
						Shaders::EnvironmentLightShader::fragmentShaderSourceGLSL) &&

		addShaderHelper("SSAO",
						Shaders::SSAO::vertexShaderSourceGLSL,
						Shaders::SSAO::fragmentShaderSourceGLSL) &&

		addShaderHelper("AverageBlur",
						Shaders::AverageBlur::vertexShaderSourceGLSL,
						Shaders::AverageBlur::fragmentShaderSourceGLSL) &&

		addShaderHelper("DepthShader",
						Shaders::DepthShader::vertexShaderSourceGLSL,
						Shaders::DepthShader::fragmentShaderSourceGLSL) &&

		addShaderHelper("ToneMapping",
						Shaders::ToneMappingShader::vertexShaderSourceGLSL,
						Shaders::ToneMappingShader::fragmentShaderSourceGLSL) &&

		addShaderHelper("GammaCorrection",
						Shaders::GammaCorrectionShader::vertexShaderSourceGLSL,
						Shaders::GammaCorrectionShader::fragmentShaderSourceGLSL) &&

		addShaderHelper("FXAA",
						Shaders::FXAAShader::vertexShaderSourceGLSL,
						Shaders::FXAAShader::fragmentShaderSourceGLSL);
}
