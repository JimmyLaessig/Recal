module;

#include <VulkanHeader.h>

#include <spirv_reflect.h>

#include <algorithm>
#include <assert.h>
#include <optional>
#include <span>
#include <string>
#include <string_view>
#include <vector>

module Coral.Vulkan.ShaderModule;

import Coral.Vulkan.Context;
import Coral.Types;
import Finally;

using namespace Coral::Vulkan;

std::optional<Coral::AttributeFormat>
convert(SpvReflectFormat format)
{
	switch (format)
	{
		case SPV_REFLECT_FORMAT_R16_UINT:				return Coral::AttributeFormat::UINT16;
		case SPV_REFLECT_FORMAT_R32_UINT:				return Coral::AttributeFormat::UINT32;
		case SPV_REFLECT_FORMAT_R16_SINT:				return Coral::AttributeFormat::INT16;
		case SPV_REFLECT_FORMAT_R32_SINT:				return Coral::AttributeFormat::INT32;
		case SPV_REFLECT_FORMAT_R32_SFLOAT:				return Coral::AttributeFormat::FLOAT;
		case SPV_REFLECT_FORMAT_R32G32_SFLOAT:			return Coral::AttributeFormat::VEC2;
		case SPV_REFLECT_FORMAT_R32G32B32_SFLOAT:		return Coral::AttributeFormat::VEC3;
		case SPV_REFLECT_FORMAT_R32G32B32A32_SFLOAT:	return Coral::AttributeFormat::VEC4;
		default:
			return {};
	}
}


Coral::UniformBlockDescription createUniformBlockDescription(const SpvReflectDescriptorBinding& binding)
{
	assert(binding.descriptor_type == SPV_REFLECT_DESCRIPTOR_TYPE_UNIFORM_BUFFER);
	assert(binding.type_description->type_flags & SPV_REFLECT_TYPE_FLAG_STRUCT);

	Coral::UniformBlockDescription result;
	
	for (uint32_t i = 0; i < binding.block.member_count; ++i)
	{
		auto& member = binding.block.members[i];

		auto& description = result.members.emplace_back();
		description.name = member.name;
		description.count = 1;

		auto& traits = member.type_description->traits;

		constexpr auto BoolFlags	= SPV_REFLECT_TYPE_FLAG_BOOL;
		constexpr auto IntFlags		= SPV_REFLECT_TYPE_FLAG_INT;
		constexpr auto FloatFlags	= SPV_REFLECT_TYPE_FLAG_FLOAT;
		constexpr auto MatrixFlags  = SPV_REFLECT_TYPE_FLAG_MATRIX | SPV_REFLECT_TYPE_FLAG_VECTOR | SPV_REFLECT_TYPE_FLAG_FLOAT;
		constexpr auto VectorFlags	= SPV_REFLECT_TYPE_FLAG_VECTOR | SPV_REFLECT_TYPE_FLAG_FLOAT;

		switch (member.type_description->type_flags)
		{
			case BoolFlags:
				description.type = Coral::ValueType::BOOL;
				break;
			case IntFlags:
				description.type = Coral::ValueType::INT;
				break;
			case FloatFlags:
				description.type = Coral::ValueType::FLOAT;
				break;
			case VectorFlags:
				switch (traits.numeric.vector.component_count)
				{
					case 2:
						description.type = Coral::ValueType::VEC2F;
						break;
					case 3:
						description.type = Coral::ValueType::VEC3F;
						break;
					case 4:
						description.type = Coral::ValueType::VEC4F;
						break;
					default:
						assert(false);
				}
				break;
			case MatrixFlags:
				if (traits.numeric.matrix.column_count == 4 && traits.numeric.matrix.row_count == 4)
				{
					description.type = Coral::ValueType::MAT44F;
				}
				else if (traits.numeric.matrix.column_count == 3 && traits.numeric.matrix.row_count == 3)
				{
					description.type = Coral::ValueType::MAT33F;
				}
				else
				{
					assert(false);
				}
				break;
			default:
				assert(false);
		}
	}

	return result;
}


bool
ShaderModule::reflect(std::span<const std::byte> spirvCode)
{
	SpvReflectShaderModule module{};

	if (spvReflectCreateShaderModule(spirvCode.size_bytes(), spirvCode.data(), &module) != SPV_REFLECT_RESULT_SUCCESS)
	{
		return false;
	}
	//Finally finally = [&] { spvReflectDestroyShaderModule(&module); };

	mEntryPoint = module.entry_point_name;

	std::vector<SpvReflectDescriptorSet*> sets;
	{
		uint32_t count{ 0 };
		spvReflectEnumerateDescriptorSets(&module, &count, nullptr);
		sets.resize(count);
		spvReflectEnumerateDescriptorSets(&module, &count, sets.data());
	}


	for (auto set : sets)
	{
		for (size_t i = 0; i < set->binding_count; ++i)
		{
			auto binding = set->bindings[i];

			auto& bindingDescription = mBindingDescriptions.emplace_back();
			bindingDescription.set		= set->set;
			bindingDescription.binding  = binding->binding;
			
			switch (binding->descriptor_type)
			{
				case SPV_REFLECT_DESCRIPTOR_TYPE_UNIFORM_BUFFER:
					
					bindingDescription.name = binding->type_description->type_name;
					bindingDescription.type = createUniformBlockDescription(*binding);
					break;
				case SPV_REFLECT_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER:
					bindingDescription.name = binding->name;
					bindingDescription.type = SamplerDescription{};
					break;
				default:
					assert(false);
			}
		}
	}

	std::vector<SpvReflectInterfaceVariable*> inputVariables;
	{
		uint32_t count{ 0 };
		spvReflectEnumerateInputVariables(&module, &count, nullptr);
		inputVariables.resize(count);
		spvReflectEnumerateInputVariables(&module, &count, inputVariables.data());
	}

	for (auto variable : inputVariables)
	{
		auto format = convert(variable->format);
		if (!format)
		{
			// TODO Log unsupported input format
			assert(false);
			return false;
		}

		auto& description		= mInputDescriptions.emplace_back();
		description.location	= variable->location;
		description.name		= variable->name;
		description.format		= *format;
	}

	std::sort(mInputDescriptions.begin(), mInputDescriptions.end(), 
			  [](const AttributeDescription& lhs, const AttributeDescription& rhs) { return lhs.location < rhs.location; });

	uint32_t binding{ 0 };
	for (auto& description : mInputDescriptions)
	{
		description.binding = binding++;
	}

	std::vector<SpvReflectInterfaceVariable*> outputVariables;
	{
		uint32_t count{ 0 };
		spvReflectEnumerateOutputVariables(&module, &count, nullptr);
		outputVariables.resize(count);
		spvReflectEnumerateOutputVariables(&module, &count, outputVariables.data());
	}

	for (auto variable : outputVariables)
	{
		if (variable->format == SPV_REFLECT_FORMAT_UNDEFINED)
		{
			continue;
		}

		auto format = convert(variable->format);
		if (!format)
		{
			// TODO Log unsupported format
			assert(false);
			return false;
		}

		auto& description		= mOutputDescriptions.emplace_back();
		description.location	= variable->location;
		description.name		= variable->name;
		description.format		= *format;
	}

	return true;
}


bool
ShaderModule::init(Context* context, const ShaderModuleConfig& config)
{
	if (!context)
	{
		return false;
	}

	mContext		= context;
	mName			= config.name;
	mShaderStage	= config.stage;

	VkShaderModuleCreateInfo createInfo{ VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO };
	createInfo.pCode	= (uint32_t*)config.source.data();
	createInfo.codeSize = config.source.size();

	if (vkCreateShaderModule(mContext->getVkDevice(), &createInfo, nullptr, &mShaderModule) != VK_SUCCESS)
	{
		return false;
	}

	if (!reflect(config.source))
	{
		return false;
	}

	return true;
}


ShaderModule::~ShaderModule()
{
	if (mContext && mShaderModule != VK_NULL_HANDLE)
	{
		vkDestroyShaderModule(mContext->getVkDevice(), mShaderModule, nullptr);
	}
}


std::string_view
ShaderModule::name() const
{
	return mName;
}


Coral::ShaderStage
ShaderModule::shaderStage() const
{
	return mShaderStage;
}


std::string_view
ShaderModule::entryPoint() const
{
	return mEntryPoint;
}


std::span<const Coral::AttributeDescription>
ShaderModule::inputDescriptions() const
{
	return mInputDescriptions;
}


std::span<const Coral::AttributeDescription>
ShaderModule::outputDescriptions() const
{
	return mOutputDescriptions;
}


std::span<const Coral::BindingDescription>
ShaderModule::bindingDescriptions() const
{
	return mBindingDescriptions;
}


VkShaderModule
ShaderModule::getVkShaderModule()
{
	return mShaderModule;
}