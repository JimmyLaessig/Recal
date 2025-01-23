#ifndef MATERIALGENERATOR_MATERIALGENERATOR_H_
#define MATERIALGENERATOR_MATERIALGENERATOR_H_

#include <MaterialGenerator/System.hpp>

#include <optional>
#include <string>
#include <vector>
#include <filesystem>
#include <unordered_map>



enum class BlendMode
{
	TRANSPARENT,
	MASKED,
	OPAQUE,
};

struct MaterialCreateInfo
{
	std::string name;
	std::string materialSource;
};


struct CompiledShaderStageInfo
{
	std::vector<uint32_t> spirvCode;
	std::string entryPoint;
};


struct GeneratedMaterialInfo
{
	/// Name of the shader
	std::string name;
	/// The compiled vertex shader source
	CompiledShaderStageInfo vertexShaderStage;
	/// The compiled fragment shader source
	CompiledShaderStageInfo fragmentShaderStage;
	/// Blend mode
	BlendMode blendMode;
};

namespace Reef
{

std::optional<GeneratedMaterialInfo> MATERIALGENERATOR_API generateMaterial(const MaterialCreateInfo& program);

} // namespace Reef


#endif // !MATERIALGENERATOR_MATERIALGENERATOR_H_