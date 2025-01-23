#ifndef SHADERCOMPILER_HEADERGENERATOR_H_
#define SHADERCOMPILER_HEADERGENERATOR_H_

#include <ShaderCompiler/ShaderCompiler.h>

#include <optional>
#include <string>

namespace ShaderCompiler
{

std::string SHADERCOMPILER_API buildShaderHeader(std::string_view className, 
												 const std::optional<std::string_view>& classNamespace,
												 const ShaderCompiler::ShaderModuleCompileInfo& compileInfo,
												 const ShaderCompiler::CompiledShaderModule& shaderProgram);

} // namespace ShaderCompiler

#endif // !SHADERCOMPILER_HEADERGENERATOR_H_