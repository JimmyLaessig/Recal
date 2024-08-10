module;

#include <Engine/System.h>

#include <string_view>
#include <unordered_map>
#include <memory>

export module Engine.ShaderManagerImpl;

import Engine.ShaderManager;
import Engine.ShaderImpl;

export namespace Reef
{
class ApplicationImpl;

class ENGINE_API ShaderManagerImpl: public IShaderManager
{
public:

	ShaderManagerImpl(ApplicationImpl& application);

	Shader* getShader(std::string_view name) override;

	Shader* createShader(std::string_view name,
						 std::span<const std::byte> vertexShaderSource,
						 std::span<const std::byte> fragmentShaderSource) override;

private:

	ApplicationImpl* mApplication;

	std::unordered_map<std::string, std::unique_ptr<ShaderImpl>> mShaders;
};

} // export namespace Reef