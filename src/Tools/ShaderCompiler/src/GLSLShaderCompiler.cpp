#include "GLSLShaderCompiler.h"

#include <optional>
#include <span>
#include <unordered_map>
#include <array>
#include <string_view>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <iostream>
#include <functional>

#include <GL/glew.h>
#include <glfw/glfw3.h>

#include <shaderc/shaderc.h>
#include <cassert>

#include "GLSLShaderIncluder.h"

namespace
{

shaderc_shader_kind
convert(ShaderType type)
{
	switch (type)
	{
	case ShaderType::VERTEX:				return shaderc_vertex_shader;
	case ShaderType::FRAGMENT:				return shaderc_fragment_shader;
	case ShaderType::GEOMETRY:				return shaderc_glsl_fragment_shader;
	case ShaderType::COMPUTE:				return shaderc_compute_shader;
	case ShaderType::TESSELLATION_CONTROL:	return shaderc_tess_control_shader;
	case ShaderType::TESSELLATION_EVAL:		return shaderc_tess_evaluation_shader;
	default:
		assert(false);
		return {};
	}
}


bool
createShader(const char* shaderSource, GLenum shaderType, GLint& handle, GLint programHandle)
{
	handle = glCreateShader(shaderType);

	glShaderSource(handle, 1, &shaderSource, nullptr);
	glCompileShader(handle);

	GLint success = GL_FALSE;

	glGetShaderiv(handle, GL_COMPILE_STATUS, &success);

	if (success == GL_FALSE)
	{
		GLint logSize{ 0 };
		glGetShaderiv(handle, GL_INFO_LOG_LENGTH, &logSize);

		std::string message(static_cast<size_t>(logSize), ' ');

		glGetShaderInfoLog(handle, logSize, NULL, message.data());

		std::cerr << "Failed to compile shader: " << message << std::endl << shaderSource << std::endl;


		return false;
	}

	glAttachShader(programHandle, handle);

	return true;
}

} // namespace


bool
GLSLShaderCompiler::init()
{
	if (glfwInit() != GLFW_TRUE)
	{
		return false;
	}

	// Create a debug Vulkan context or tell your Vulkan library (GLFW, SDL) to do so.
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
	// Set window hints
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_REFRESH_RATE, 60);
	glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);

	mHeadlessContext = glfwCreateWindow(1, 1, "", nullptr, nullptr);

	if (!mHeadlessContext)
	{
		return false;
	}

	glfwMakeContextCurrent(mHeadlessContext);

	if (glewInit() != GLEW_OK)
	{
		return false;
	}

	return true;
}


GLSLShaderCompiler::~GLSLShaderCompiler()
{
	if (mHeadlessContext)
	{
		glfwDestroyWindow(mHeadlessContext);
	}
}


struct ScopeExit
{
	template<typename FUNC>
	ScopeExit(FUNC&& fun) : mFun(fun)
	{}


	~ScopeExit() { mFun(); }

	std::function<void()> mFun;
};


std::optional<std::vector<uint32_t>>
compileShaderToSPV(const std::string& shaderSource, const std::string& filename, ShaderType type)
{
	
	auto options = shaderc_compile_options_initialize();
	shaderc_compile_options_set_source_language(options, shaderc_source_language_glsl);
	shaderc_compile_options_set_target_env(options, shaderc_target_env_vulkan, 0);
	shaderc_compile_options_set_target_spirv(options, shaderc_spirv_version_1_3);

	auto compiler = shaderc_compiler_initialize();
	
	auto result = shaderc_compile_into_spv(compiler,
		shaderSource.c_str(),
		shaderSource.size(),
		convert(type),
		filename.c_str(),
		"main",
		options);

	std::optional<std::string> res;
	if (shaderc_result_get_compilation_status(result) != shaderc_compilation_status_success)
	{
		std::cerr << "Failed to compile shader to SpirV: " << shaderc_result_get_error_message(result) << std::endl;
		return {};
	}

	auto bytes = shaderc_result_get_bytes(result);
	auto size = shaderc_result_get_length(result);
	assert(size % sizeof(uint32_t) == 0);
	std::vector<uint32_t> compiled(size / sizeof(uint32_t));
	std::memcpy(compiled.data(), bytes, size);

	return { std::move(compiled) };
}


bool
GLSLShaderCompiler::compileShaderGLSL(const ShaderProgram& shaderProgram)
{
	auto programHandle = glCreateProgram();

	GLint vertexHandle{ -1 };
	GLint fragmentHandle{ -1 };
	GLint geometryHandle{ -1 };
	GLint tessControlHandle{ -1 };
	GLint tessEvalHandle{ -1 };
	GLint computeHandle{ -1 };

	ScopeExit cleanup([&]
		{
			if (programHandle >= 0)
			{
				glDeleteShader(programHandle);
			}

			for (auto handle : { vertexHandle,
								 fragmentHandle,
								 geometryHandle,
								 tessControlHandle,
								 tessEvalHandle,
								 computeHandle })
			{
				if (handle >= 0)
				{
					glDeleteShader(handle);
				}
				handle = -1;
			}
		});


	if (shaderProgram.vertexShader && !createShader(shaderProgram.vertexShader->glslCode.c_str(), GL_VERTEX_SHADER, vertexHandle, programHandle))
	{
		return false;
	}


	if (shaderProgram.fragmentShader && !createShader(shaderProgram.fragmentShader->glslCode.c_str(), GL_FRAGMENT_SHADER, fragmentHandle, programHandle))
	{
		return false;
	}


	if (shaderProgram.geometryShader && !createShader(shaderProgram.geometryShader->glslCode.c_str(), GL_GEOMETRY_SHADER, geometryHandle, programHandle))
	{
		return false;
	}


	if (shaderProgram.tessControlShader && !createShader(shaderProgram.tessControlShader->glslCode.c_str(), GL_TESS_CONTROL_SHADER, tessControlHandle, programHandle))
	{
		return false;
	}

	if (shaderProgram.tessEvalShader && !createShader(shaderProgram.tessEvalShader->glslCode.c_str(), GL_TESS_EVALUATION_SHADER, tessEvalHandle, programHandle))
	{
		return false;
	}

	if (shaderProgram.computeShader && !createShader(shaderProgram.computeShader->glslCode.c_str(), GL_COMPUTE_SHADER, computeHandle, programHandle))
	{
		return false;
	}

	glLinkProgram(programHandle);

	GLint success{ GL_FALSE };
	glGetProgramiv(programHandle, GL_LINK_STATUS, &success);

	if (success == GL_FALSE)
	{
		GLint logSize{ 0 };
		glGetProgramiv(programHandle, GL_INFO_LOG_LENGTH, &logSize);

		std::string message(logSize, ' ');
		GLsizei length{ 0 };
		glGetProgramInfoLog(programHandle, logSize, &length, message.data());

		std::cerr << "Failed to link shader program: " << message << std::endl;

		return false;
	}

	return true;
}


bool
GLSLShaderCompiler::compileShaderSpirV(ShaderProgram& shaderProgram)
{	
	constexpr auto filePath = "shader.glsl";

	if (shaderProgram.vertexShader)
	{
		if (auto res = compileShaderToSPV(shaderProgram.vertexShader->glslCode, filePath, ShaderType::VERTEX))
		{
			shaderProgram.vertexShader->spirvCode = std::move(*res);
		}
		else
		{
			return false;
		}
	}

	if (shaderProgram.geometryShader)
	{
		if (auto res = compileShaderToSPV(shaderProgram.geometryShader->glslCode, filePath, ShaderType::GEOMETRY))
		{
			shaderProgram.geometryShader->spirvCode = std::move(*res);
		}
		else
		{
			return false;
		}
	}

	if (shaderProgram.tessControlShader)
	{
		if (auto res = compileShaderToSPV(shaderProgram.tessControlShader->glslCode, filePath, ShaderType::TESSELLATION_CONTROL))
		{
			shaderProgram.tessControlShader->spirvCode = std::move(*res);
		}
		else
		{
			return false;
		}
	}

	if (shaderProgram.tessEvalShader)
	{
		if (auto res = compileShaderToSPV(shaderProgram.tessEvalShader->glslCode, filePath, ShaderType::TESSELLATION_EVAL))
		{
			shaderProgram.tessEvalShader->spirvCode = std::move(*res);
		}
		else
		{
			return false;
		}
	}
	

	if (shaderProgram.fragmentShader)
	{
		if (auto res = compileShaderToSPV(shaderProgram.fragmentShader->glslCode, filePath, ShaderType::FRAGMENT))
		{
			shaderProgram.fragmentShader->spirvCode = std::move(*res);
		}
		else
		{
			return false;
		}
	}

	if (shaderProgram.computeShader)
	{
		if (auto res = compileShaderToSPV(shaderProgram.computeShader->glslCode, filePath, ShaderType::COMPUTE))
		{
			shaderProgram.computeShader->spirvCode = std::move(*res);
		}
		else
		{
			return false;
		}
	}

	return true;
}


bool
GLSLShaderCompiler::compileShader(ShaderProgram& shaderProgram)
{
	return compileShaderGLSL(shaderProgram) && compileShaderSpirV(shaderProgram);
}