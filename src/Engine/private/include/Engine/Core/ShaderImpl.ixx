module;

#include <Engine/System.h>

#include <string_view>
#include <unordered_map>
#include <memory>

export module Engine.ShaderImpl;

import Engine.Shader;

import Coral.RAII;

export namespace Reef
{

class ENGINE_API ShaderImpl: public Shader
{
public:

	const std::string& name() const override;

private:

	friend class ShaderManagerImpl;

	std::string mName;

	Coral::RAII::ShaderModulePtr mVertexShader;

	Coral::RAII::ShaderModulePtr mFragmentShader;
	
};

} // export namespace Reef