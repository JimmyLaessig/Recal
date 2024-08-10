#ifndef SHADERCOMPILER_GLSLSHADERINCLUDER_H_
#define SHADERCOMPILER_GLSLSHADERINCLUDER_H_

#include <filesystem>
#include <optional>
#include <string>


#include <shaderc/shaderc.h>

enum class ShaderType
{
	VERTEX,
	GEOMETRY,
	FRAGMENT,
	COMPUTE,
	TESSELLATION_CONTROL,
	TESSELLATION_EVAL,
};

class GLSLShaderIncluder
{
public:

	GLSLShaderIncluder(const std::vector<std::filesystem::path>& workingDirectories);

	std::optional<std::string> resolveIncludes(const std::string& content, 
											   const std::filesystem::path& filePath, 
											   ShaderType type);

private:

	static shaderc_include_result* resolveInclude(void* userData, 
												  const char* requestedSource, 
												  int type,
												  const char* requestingSsource, 
												  size_t includeDepth);

	static void releaseInclude(void* userData,
							   shaderc_include_result* includeResult);

	std::vector<std::filesystem::path> mWorkingDirectories;
};

#endif // SHADERCOMPILER_GLSLSHADERINCLUDER_H_