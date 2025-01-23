#ifndef ENGINE_RENDERING_SHADER_HPP
#define ENGINE_RENDERING_SHADER_HPP

#include <Engine/System.hpp>

#include <string>
#include <vector>
#include <span>

#include <Coral/ShaderModule.hpp>

namespace Reef::Rendering
{

class ENGINE_API Shader
{
public:

	virtual ~Shader() = default;

	virtual const std::string& name() const = 0;

	/// Get a list of descriptor binding definitions required by this shader
	virtual std::vector<Coral::DescriptorBindingDefinition> descriptorBindingDefinitions() const = 0;
};

} // namespace Reef

#endif // !ENGINE_RENDERING_SHADER_HPP