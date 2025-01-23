#include <Engine/Core/Material.hpp>

#include <glm/glm.hpp>

#include <string>
#include <memory>
#include <cassert>

#include <Common/Visitor.hpp>

using namespace Reef;


//Material::Material(std::shared_ptr<Reef::Rendering::Shader> shader)
//	: mShader(shader)
//{
//}


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


std::optional<Material::ParameterValue>
Material::getParameter(std::string_view name) const
{
	auto iter = mParameters.find(std::string(name));

	if (iter == mParameters.end())
	{
		return {};
	}

	return iter->second;
}


bool
Material::setParameterImpl(std::string_view name, Material::ParameterValue value)
{
	auto iter = mParameters.find(std::string(name));

	if (iter == mParameters.end())
	{
		return false;
	}

	if (iter->second.index() != value.index())
	{
		return false;
	}

	iter->second = value;

	markOutdated();
	return true;
}


//std::shared_ptr<Material>
//Material::DefaultPBR()
//{
//	std::shared_ptr<Material> sMaterial;
//
//	if (!sMaterial)
//	{
//		sMaterial = std::make_shared<Material>();
//		//sMaterial->setShader(Engine::Rendering::Renderer::instance().shaderManager().find("DefaultPBR"));
//		sMaterial->setParameter("baseColorFactor", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
//		sMaterial->setParameter("baseColorTexture", Reef::Texture::white());
//		sMaterial->setParameter("normalTexture", Reef::Texture::normal());
//		sMaterial->setParameter("occlusionTexture", Reef::Texture::black());
//		sMaterial->setParameter("occlusionChannel", 0);
//		sMaterial->setParameter("roughnessTexture", Reef::Texture::white());
//		sMaterial->setParameter("roughnessChannel", 1);
//		sMaterial->setParameter("roughnessFactor", 1.0f);
//		sMaterial->setParameter("metallicTexture", Reef::Texture::black());
//		sMaterial->setParameter("metallicChannel", 2);
//		sMaterial->setParameter("metallicFactor", 0.f);
//	}
//
//	return sMaterial;
//}


void
Material::setShader(std::shared_ptr<Reef::Rendering::Shader> shader)
{
	mShader = shader;
	mParameters.clear();
	markOutdated();

	if (!mShader)
	{
		return;
	}

	for (const auto& definition : mShader->descriptorBindingDefinitions())
	{
		Common::visit(definition.definition, Common::Visitor{
			[this, &definition](const Coral::SamplerDefinition& sampler)
			{ 
				mParameters[definition.name] = Texture::White;
			},
			[this](const Coral::UniformBlockDefinition& uniformBlock)
			{
				for (const auto& member : uniformBlock.members)
				{
					switch (member.type)
					{
						case Coral::ValueType::BOOL:	mParameters[member.name] = true; break;
						case Coral::ValueType::INT:		mParameters[member.name] = 0; break;
						case Coral::ValueType::FLOAT:	mParameters[member.name] = 0.f; break;
						case Coral::ValueType::MAT33F:	mParameters[member.name] = glm::mat3(1.f); break;
						case Coral::ValueType::MAT44F:	mParameters[member.name] = glm::mat4(1.f); break;
						case Coral::ValueType::VEC2F:	mParameters[member.name] = glm::vec2(0.f, 0.f); break;
						case Coral::ValueType::VEC3F:	mParameters[member.name] = glm::vec3(0.f, 0.f, 0.f); break;
						case Coral::ValueType::VEC4F:	mParameters[member.name] = glm::vec4(0.f, 0.f, 0.f, 0.f); break;
					}
				}
			} });
	}
}


std::shared_ptr<Reef::Rendering::Shader>
Material::shader() const
{
	return mShader;
}