module;

#include <string_view>

module Engine.Rendering.ShaderManager;


import RenderLib.RAII;


using namespace Reef::Rendering;


std::shared_ptr<Engine::Shader>
ShaderManager::addShader(std::string_view name,
						 std::string_view vertexShaderSource,
						 std::string_view fragmentShaderSource)
{
	RenderLib::ShaderConfig config{};
	config.name = name;
	config.vertexShaderSource = vertexShaderSource;
	config.fragmentShaderSource = fragmentShaderSource;

	auto shaderProgram = RenderLib::RAII::createShader(mContext, config);

	if (!shaderProgram)
	{
		return nullptr;
	}

	std::shared_ptr<Engine::Shader> shader(new Engine::Shader(name, std::move(shaderProgram)));
	
	mShaders[std::hash<std::string_view>()(name)] = shader;

	return shader;
}


std::shared_ptr<Engine::Shader>
ShaderManager::find(std::string_view name)
{
	auto iter = mShaders.find(std::hash<std::string_view>()(name));

	if (iter == mShaders.end())
	{
		return nullptr;
	}

	return iter->second;
}


bool
ShaderManager::initialize(RenderLib::Context& context)
{
	mContext = &context;
	return true;
}
