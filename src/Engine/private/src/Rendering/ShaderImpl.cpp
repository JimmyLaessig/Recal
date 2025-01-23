#include <Engine/Rendering/ShaderImpl.hpp>

#include <string>
#include <string_view>
#include <memory>

using namespace Reef::Rendering;


std::unique_ptr<ShaderImpl>
ShaderImpl::create(std::string_view name, std::span<const std::byte> vertexShaderSource, std::span<const std::byte> fragmentShaderSource, Coral::Context& context)
{
	std::unique_ptr<ShaderImpl> shader(new ShaderImpl);

	shader->mName = name;

	std::string vertexShaderName = shader->mName + "_VertexShader";
	Coral::ShaderModuleConfig vertexShaderConfig{};
	vertexShaderConfig.name		= vertexShaderName;
	vertexShaderConfig.stage	= Coral::ShaderStage::VERTEX;
	vertexShaderConfig.source	= vertexShaderSource;
	shader->mVertexShader		= Coral::RAII::createShaderModule(&context, vertexShaderConfig);

	if (!shader->mVertexShader)
	{
		return nullptr;
	}

	std::string fragmentShaderName = shader->mName + "_FragmentShader";
	Coral::ShaderModuleConfig fragmentShaderConfig{};
	fragmentShaderConfig.name	= fragmentShaderName;
	fragmentShaderConfig.stage	= Coral::ShaderStage::FRAGMENT;
	fragmentShaderConfig.source = fragmentShaderSource;
	shader->mFragmentShader		= Coral::RAII::createShaderModule(&context, fragmentShaderConfig);

	if (!shader->mFragmentShader)
	{
		return nullptr;
	}

	return shader;
}


const std::string&
ShaderImpl::name() const
{
	return mName;
}


Coral::ShaderModule*
ShaderImpl::vertexShader() const
{
	return mVertexShader.get();
}


Coral::ShaderModule*
ShaderImpl::fragmentShader() const
{
	return mFragmentShader.get();
}


std::vector<Coral::DescriptorBindingDefinition>
ShaderImpl::descriptorBindingDefinitions() const
{
	std::vector<Coral::DescriptorBindingDefinition> result;

	for (const auto& shader : { mVertexShader.get(), mFragmentShader.get() })
	{
		for (const auto& definition : shader->descriptorBindingDefinitions())
		{
			result.push_back(definition);
		}
	}

	return result;
}