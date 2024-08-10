module;

#include <Engine/System.h>

#include <memory>
#include <string_view>
#include <span>

export module Engine.ShaderManager;

import Engine.Shader;

export namespace Reef
{

namespace BuiltInShaders
{
	constexpr auto DefaultPBR = "DefaultPBR";
}


class ENGINE_API IShaderManager
{
public:

	virtual Shader* createShader(std::string_view name, 
							     std::span<const std::byte> vertexShaderSource,
								 std::span<const std::byte> fragmentShaderSource) = 0;

	virtual Shader* getShader(std::string_view name) = 0;
};

} // export namespace Reef