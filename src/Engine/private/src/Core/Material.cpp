module;

#include <glm/glm.hpp>

#include <string>
#include <memory>
#include <cassert>

module Engine.Material;

import Engine.Texture;

using namespace Reef;


Material::Material()
{
	//setShader(Engine::Rendering::Renderer::instance().shaderManager().find("DefaultPBR"));
}


bool
Material::setParameter(std::string_view name, float value)
{
	return setParameterImpl(name, value);
}


bool
Material::setParameter(std::string_view name, const glm::vec2& value)
{
	return setParameterImpl(name, value);
}


bool
Material::setParameter(std::string_view name, const glm::vec3& value)
{
	return setParameterImpl(name, value);
}


bool
Material::setParameter(std::string_view name, const glm::vec4& value)
{
	return setParameterImpl(name, value);
}

bool
Material::setParameter(std::string_view name, int value)
{
	return setParameterImpl(name, value);
}


bool
Material::setParameter(std::string_view name, const glm::ivec2& value)
{
	return setParameterImpl(name, value);
}


bool
Material::setParameter(std::string_view name, const glm::ivec3& value)
{
	return setParameterImpl(name, value);
}


bool
Material::setParameter(std::string_view name, const glm::ivec4& value)
{
	return setParameterImpl(name, value);
}

bool
Material::setParameter(std::string_view name, const glm::mat2& value)
{
	return setParameterImpl(name, value);
}


bool
Material::setParameter(std::string_view name, const glm::mat3& value)
{
	return setParameterImpl(name, value);
}


bool
Material::setParameter(std::string_view name, const glm::mat4& value)
{
	return setParameterImpl(name, value);
}


bool
Material::setParameter(std::string_view name, std::shared_ptr<Texture> value)
{
	return setParameterImpl(name, value);
}


void 
Material::setMode(Mode mode)
{
	mMode = mode;
}


Material::Mode
Material::mode() const
{
	return mMode;
}


//void
//Material::setShader(std::shared_ptr<Shader> shader)
//{
//	mShader = shader;
//	mNameToLocationMapping.clear();
//	mParameters.clear();
//	for (auto& binding : mShader->shader()->getUniformBindings())
//	{
//		mNameToLocationMapping[binding.definition.name] = binding.location;
//		switch (binding.definition.type)
//		{
//			case RenderLib::UniformType::FLOAT:		setParameter(binding.definition.name, 0.f); break;
//			case RenderLib::UniformType::VEC2F:		setParameter(binding.definition.name, glm::vec2()); break;
//			case RenderLib::UniformType::VEC3F:		setParameter(binding.definition.name, glm::vec3()); break;
//			case RenderLib::UniformType::VEC4F:		setParameter(binding.definition.name, glm::vec4()); break;
//			case RenderLib::UniformType::MAT2X2F:	setParameter(binding.definition.name, glm::mat2()); break;
//			//case RenderLib::UniformType::MAT2X3F:	setParameter(binding.definition.name, Math::Mat23F); break;
//			//case RenderLib::UniformType::MAT2X4F:	setParameter(binding.definition.name, Math::Mat24F); break;
//			//case RenderLib::UniformType::MAT3X2F:	setParameter(binding.definition.name, Math::Mat32F); break;
//			case RenderLib::UniformType::MAT3X3F:	setParameter(binding.definition.name, glm::mat3()); break;
//			//case RenderLib::UniformType::MAT3X4F:	setParameter(binding.definition.name, Math::Mat34F); break;
//			//case RenderLib::UniformType::MAT4X2F:	setParameter(binding.definition.name, Math::Mat42F); break;
//			//case RenderLib::UniformType::MAT4X3F:	setParameter(binding.definition.name, Math::Mat43F); break;
//			case RenderLib::UniformType::MAT4X4F:	setParameter(binding.definition.name, glm::mat4()); break;
//		/*	case RenderLib::UniformType::UINT32:	setParameter(binding.definition.name, 0u); break;
//			case RenderLib::UniformType::UINT32_2:  setParameter(binding.definition.name, glm::uvec2()); break;
//			case RenderLib::UniformType::UINT32_3:  setParameter(binding.definition.name, glm::uvec3()); break;
//			case RenderLib::UniformType::UINT32_4:  setParameter(binding.definition.name, glm::uvec4()); break;*/
//			case RenderLib::UniformType::INT32:		setParameter(binding.definition.name, 0); break;
//			//case RenderLib::UniformType::INT32_2:   setParameter(binding.definition.name, glm::ivec2); break;
//			//case RenderLib::UniformType::INT32_3:   setParameter(binding.definition.name, glm::ivec3); break;
//			//case RenderLib::UniformType::INT32_4:   setParameter(binding.definition.name, glm::ivec4); break;
//			case RenderLib::UniformType::SAMPLER:   setParameter(binding.definition.name, Engine::Texture::black()); break;
//			default: assert(false && "unsupported uniform value");
//		}
//	}
//}


//std::shared_ptr<Shader> 
//Material::shader()
//{
//	return mShader;
//}

std::unordered_map<uint32_t, Material::ParameterValue>
Material::parameters() const
{ 
	return mParameters;
}


bool
Material::setParameterImpl(std::string_view name, Material::ParameterValue value)
{
	std::string nameVal(name.data(), name.size());
	auto iter = mNameToLocationMapping.find(nameVal);

	if (iter == mNameToLocationMapping.end())
	{
		return false;
	}

	// TODO: Check that the uniform type matches
	mParameters[iter->second] = value;

	mIsOutdated = true;
	return true;
}


std::shared_ptr<Material>
Material::DefaultPBR()
{
	std::shared_ptr<Material> sMaterial;

	if (!sMaterial)
	{
		sMaterial = std::make_shared<Material>();
		//sMaterial->setShader(Engine::Rendering::Renderer::instance().shaderManager().find("DefaultPBR"));
		sMaterial->setParameter("baseColorFactor", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
		sMaterial->setParameter("baseColorTexture", Reef::Texture::white());
		sMaterial->setParameter("normalTexture", Reef::Texture::normal());
		sMaterial->setParameter("occlusionTexture", Reef::Texture::black());
		sMaterial->setParameter("occlusionChannel", 0);
		sMaterial->setParameter("roughnessTexture", Reef::Texture::white());
		sMaterial->setParameter("roughnessChannel", 1);
		sMaterial->setParameter("roughnessFactor", 1.0f);
		sMaterial->setParameter("metallicTexture", Reef::Texture::black());
		sMaterial->setParameter("metallicChannel", 2);
		sMaterial->setParameter("metallicFactor", 0.f);
	}

	return sMaterial;
}