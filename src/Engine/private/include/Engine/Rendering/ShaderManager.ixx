module;

#include <unordered_map>
#include <string_view>
#include <memory>

export module Engine.Rendering.ShaderManager;

import Engine.Shader;

import RenderLib;

namespace Reef
{

namespace Rendering
{

export class ShaderManager
{
public:

	bool initialize(RenderLib::Context& context);

	std::shared_ptr<Engine::Shader> find(std::string_view name);

	std::shared_ptr<Engine::Shader> addShader(std::string_view name,
											  std::string_view vertexShaderSource,
											  std::string_view fragmentShaderSource);

private:

	RenderLib::Context* mContext;

	std::unordered_map<size_t, std::shared_ptr<Engine::Shader>> mShaders;

};

} // namespace Rendering

} // namespace Reef