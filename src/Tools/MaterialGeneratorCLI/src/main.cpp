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

#include <ShaderCompiler/ShaderCompiler.h>
#include <ShaderCompiler/HeaderGenerator.h>

#include <regex>

#include <CLI/CLI.hpp>


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


int main(int argc, char** argv)
{
	std::string shaderName;
	std::string shaderNamespace;
	std::filesystem::path inputPath;
	std::filesystem::path outputPath;
	std::string entryPoint;
	ShaderCompiler::ShaderStage type;
	ShaderCompiler::Language language;

	CLI::App app{ "HLSL Shader Compiler" };

	std::map<std::string, ShaderCompiler::ShaderStage> shaderStageMap{ { "vertex", ShaderCompiler::ShaderStage::VERTEX }, { "fragment", ShaderCompiler::ShaderStage::FRAGMENT } };

	std::map<std::string, ShaderCompiler::Language> languageMap{ { "glsl", ShaderCompiler::Language::GLSL }, { "fragment", ShaderCompiler::Language::HLSL } };

	app.add_option("-n,--name",		  shaderName,	   "The name of the compiled shader")->required(true);
	app.add_option("-ns,--namespace", shaderNamespace, "The optional namespace of the compiled shader header");
	app.add_option("-o,--output",	  outputPath,	   "The path to the compiled shader header")->required(true);
	app.add_option("-i,--input",	  inputPath,	   "The path to the HLSL shader file")->required(true);
	app.add_option("-e,--entrypoint", entryPoint,	   "The name of the shader entry point function")->required(true);
	app.add_option("-s,--stage",	  type,			   "The stage of the shader")->required(true)->transform(CLI::CheckedTransformer(shaderStageMap, CLI::ignore_case));
	app.add_option("-l,--language",   language,		   "The source language of the shader code")->required(true)->transform(CLI::CheckedTransformer(languageMap, CLI::ignore_case));

	try
	{
		app.parse(argc, argv);
	}
	catch (const CLI::ParseError& e)
	{
		return app.exit(e);
	}

	auto content = loadFile(inputPath);

	if (!content)
	{
		std::cerr << "Failed to open input file '" << inputPath << "'." << std::endl;
		return EXIT_FAILURE;
	}

	ShaderCompiler::ShaderModuleCompileInfo compileInfo;
	compileInfo.entryPoint  = entryPoint;
	compileInfo.sourceCode  = std::move(*content);
	compileInfo.type		= type;
	compileInfo.language    = language;
	compileInfo.additionalIncludeDirectories = { inputPath.parent_path(), std::filesystem::current_path() };

	auto compiledShader = ShaderCompiler::compileHLSLToSpirV(compileInfo);

	if (!compiledShader)
	{
		return EXIT_FAILURE;
	}
	
	std::string fileContent = ShaderCompiler::buildShaderHeader(shaderName, shaderNamespace, compileInfo, *compiledShader);

	if (!writeFile(fileContent, outputPath))
	{
		std::cerr << "Failed to write output file '" << outputPath << "'." << std::endl;
		return EXIT_FAILURE;
	}

	std::cout << "Successfully written shader '"<< shaderName << "' to '" << outputPath << "'." << std::endl;
	return EXIT_SUCCESS;
}