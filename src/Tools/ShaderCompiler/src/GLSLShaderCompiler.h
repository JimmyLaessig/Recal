#ifndef SHADERCOMPILER_GLSLSHADERCOMPILER_H_
#define SHADERCOMPILER_GLSLSHADERCOMPILER_H_

#include <optional>
#include <string>
#include <vector>
#include <filesystem>

class GLFWwindow;

struct ShaderModule
{
	std::string glslCode;
	std::vector<uint32_t> spirvCode;
	std::filesystem::path path;
};

struct ShaderProgram
{
	std::optional<ShaderModule> vertexShader;
	std::optional<ShaderModule> fragmentShader;
	std::optional<ShaderModule> computeShader;
	std::optional<ShaderModule> geometryShader;
	std::optional<ShaderModule> tessControlShader;
	std::optional<ShaderModule> tessEvalShader;
};


class GLSLShaderCompiler
{
public:

	bool init();

	~GLSLShaderCompiler();

	bool compileShader(ShaderProgram& shaderProgram);

private:

	bool compileShaderGLSL(const ShaderProgram& shaderProgram);

	bool compileShaderSpirV(ShaderProgram& shaderProgram);

	GLFWwindow* mHeadlessContext{ nullptr };
};

#endif // SHADERCOMPILER_GLSLSHADERCOMPILER_H_