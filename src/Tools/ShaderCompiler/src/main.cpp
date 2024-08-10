#include <istream>

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
#include <memory>

#include <GL/glew.h>
#include <glfw/glfw3.h>

#include "GLSLShaderCompiler.h"
#include "GLSLShaderIncluder.h"
#include "HeaderGenerator.h"

#include<regex>

#include <CLI/CLI.hpp>


struct Options
{
	std::string shaderName;

	std::optional<std::string> shaderNamespace;

	std::filesystem::path outputPath;

	std::optional<std::filesystem::path> vertexShaderPath;

	std::optional<std::filesystem::path> fragmentShaderPath;

	std::optional<std::filesystem::path> computeShaderPath;

	std::optional<std::filesystem::path> geometryShaderPath;

	std::optional<std::filesystem::path> tessControlShaderPath;

	std::optional<std::filesystem::path> tessEvalShaderPath;
};


bool
validate(const Options& arguments)
{
	// Compute shader
	if (arguments.computeShaderPath && 
		!arguments.vertexShaderPath && 
		!arguments.fragmentShaderPath && 
		!arguments.geometryShaderPath && 
		!arguments.tessControlShaderPath && 
		!arguments.tessEvalShaderPath)
	{
		return true;
	}

	// Vertex + Fragment shader
	if (arguments.vertexShaderPath && 
		arguments.fragmentShaderPath && 
		!arguments.computeShaderPath &&
		!arguments.geometryShaderPath && 
		!arguments.tessControlShaderPath && 
		!arguments.tessEvalShaderPath)
	{
		return true;
	}

	// Vertex + Geometry + Fragment shader
	if (arguments.vertexShaderPath && 
		arguments.fragmentShaderPath && 
		arguments.geometryShaderPath && 
		!arguments.computeShaderPath &&
		!arguments.tessControlShaderPath && 
		!arguments.tessEvalShaderPath)
	{
		return true;
	}

	// Vertex + TessControl * TessEval + Fragment shader
	if (arguments.vertexShaderPath &&
		arguments.tessControlShaderPath &&
		arguments.tessEvalShaderPath &&
		arguments.fragmentShaderPath &&
		!arguments.geometryShaderPath &&
		!arguments.computeShaderPath)
	{
		return true;
	}

	// Vertex + Geometry  + TessControl * TessEval + Fragment shader
	if (arguments.vertexShaderPath &&
		arguments.geometryShaderPath &&
		arguments.tessControlShaderPath &&
		arguments.tessEvalShaderPath &&
		arguments.fragmentShaderPath &&
		!arguments.computeShaderPath)
	{
		return true;
	}

	std::cerr << "Invalid shader combination" << std::endl;
	return false;
}


std::optional<std::string>
loadFile(const std::filesystem::path& path)
{
	std::ifstream file(path);
	if (!file.good())
	{
		return {};
	}

	std::stringstream buffer;
	buffer << file.rdbuf();

	return { buffer.str() };
}


bool
writeFile(std::string_view content, const std::filesystem::path& filePath)
{
	//std::filesystem::path filePath = sanitizePath(path);

	auto parentDirectory = filePath.parent_path();

	if (!std::filesystem::exists(parentDirectory) && !parentDirectory.empty())
	{
		std::filesystem::create_directories(parentDirectory);
	}

	std::ofstream f(filePath);

	if (!f.is_open())
	{
		return false;
	}

	f.write(content.data(), content.size());

	return true;
}


std::optional<ShaderProgram>
loadShaderContents(const Options& arguments)
{
	std::optional<ShaderProgram> result = ShaderProgram{};

	for (auto &[path, source, type] : { std::make_tuple(arguments.vertexShaderPath,		&result->vertexShader,		ShaderType::VERTEX),
									    std::make_tuple(arguments.fragmentShaderPath,	&result->fragmentShader,	ShaderType::FRAGMENT),
										std::make_tuple(arguments.computeShaderPath,	&result->computeShader,		ShaderType::COMPUTE),
										std::make_tuple(arguments.geometryShaderPath,	&result->geometryShader,	ShaderType::GEOMETRY),
										std::make_tuple(arguments.tessControlShaderPath,&result->tessControlShader, ShaderType::TESSELLATION_CONTROL),
										std::make_tuple(arguments.tessEvalShaderPath,	&result->tessEvalShader,	ShaderType::TESSELLATION_EVAL) })
	{
		if (!path)
		{
			continue;
		}

		auto content = loadFile(*path);

		if (!content)
		{
			std::cerr << "Failed to load shader file from '" << *path << "'." << std::endl;
			return {};
		}

		GLSLShaderIncluder includer({ std::filesystem::current_path() });

		if (auto resolved = includer.resolveIncludes(*content, *path, type))
		{
			auto& module	= source->emplace();
			module.glslCode = std::move(*resolved);
			module.path		= *path;
		}
		else
		{
			return {};
		}
	}

	return result;
}


int main(int argc, char* argv[])
{
	Options arguments;

	CLI::App app{ "Shader Compiler" };

	app.add_option("--name",		arguments.shaderName,			"The name of the compiled shader")->required(true);
	app.add_option("--namespace",	arguments.shaderNamespace,		"The optional namespace of the compiled shader header");
	app.add_option("--output",		arguments.outputPath,			"The path to the compiled shader header")->required(true);
	app.add_option("--vertex",		arguments.vertexShaderPath,		"The path to the vertex shader file");
	app.add_option("--fragment",	arguments.fragmentShaderPath,	"The path to the fragment shader file");
	app.add_option("--compute",		arguments.computeShaderPath,	"The path to the compute shader file");
	app.add_option("--geom",		arguments.computeShaderPath,	"The path to the geometry shader file");
	app.add_option("--tessControl", arguments.computeShaderPath,	"The path to the tessellation control shader file");
	app.add_option("--tessEval",	arguments.computeShaderPath,	"The path to the tessellation evaluation shader file");
	
	try
	{
		app.parse(argc, argv);
	}
	catch (const CLI::ParseError& e)
	{
		return app.exit(e);
	}

	if (!validate(arguments))
	{
		return 1;
	}

	auto shaderProgram = loadShaderContents(arguments);
	if (!shaderProgram)
	{
		return 1;
	}

	GLSLShaderCompiler compiler;

	if (compiler.init())
	{
		if (!compiler.compileShader(*shaderProgram))
		{
			return 1;
		}
	}
	else
	{
		std::cout << "GLSL Shader compiler is unavailable. Creating shader file without compile check." << std::endl;
	}

	std::string fileContent = HeaderGenerator::buildShaderHeader(arguments.shaderName, arguments.shaderNamespace, *shaderProgram);

	if (!writeFile(fileContent, arguments.outputPath))
	{
		return 1;
	}

	std::cout << "Successfully written shader '"<< arguments.shaderName << "' to '" << arguments.outputPath << "'" << std::endl;
	return 0;
}