#include <Engine/Rendering/MaterialResource.hpp>

#include <Engine/Rendering/ShaderImpl.hpp>

#include <Engine/Rendering/Shaders/DefaultUniforms.h>
#include <Engine/Rendering/ResourceManager.hpp>

#include <Common/Visitor.hpp>

#include <glm/glm.hpp>

#include <vector>
#include <memory>
#include <span>
#include <ranges>


using namespace Reef::Rendering;

namespace
{


void
updateUniformBuffer(const Coral::UniformBlockBuilder& builder, Coral::Buffer& buffer)
{
	if (auto data = buffer.map())
	{
		std::memcpy(data, builder.data().data(), buffer.size());
		buffer.unmap();
	}
}

} // namespace


MaterialResource::MaterialResource(ResourceManager& resourceManager)
	: RenderResource(resourceManager)
{
}


bool
MaterialResource::createUniformBlocks(const Reef::Material& material, Coral::Context& context)
{
	const auto& shaderImpl = static_cast<const Reef::Rendering::ShaderImpl&>(*material.shader());

	// Collect all descriptor set definitions and allocate the uniform buffers
	for (auto shader : { shaderImpl.vertexShader(), shaderImpl.fragmentShader() })
	{
		for (const auto& definition : shader->descriptorBindingDefinitions())
		{
			if (definition.set != PERMATERIAL_DESCRIPTORSET)
			{
				continue;
			}

			Common::visit(definition.definition, Common::Visitor{
				[&](const Coral::SamplerDefinition& sampler)
				{
					if (auto param = material.getParameter(definition.name))
					{
						Common::visit(*param, Common::Visitor{ 
							[&, this](std::shared_ptr<Texture> texture)
							{
								if (auto resource = resourceManager().getResource(*texture))
								{
									mDescriptorSets[definition.set].samplers[definition.binding] = resource->sampler();
								}
							},
							[](const auto&) { assert(false); }
						});
					}
				},
				[&](const Coral::UniformBlockDefinition& block)
				{
					auto& binding = mDescriptorSets[definition.set].uniforms[definition.binding];
					binding.block = Coral::UniformBlockBuilder(block);

					Coral::BufferConfig config{};
					config.cpuVisible = true;
					config.size		  = binding.block.size();
					config.type		  = Coral::BufferType::UNIFORM_BUFFER;
					binding.buffer	  = Coral::RAII::createBuffer(&context, config);
				}
			});
		}
	}

	return updateUniformBlocks(material, context);
}


bool
MaterialResource::createDescriptorSets(const Reef::Material& material, Coral::Context& context)
{
	mDescriptorSets.clear();

	if (!createUniformBlocks(material, context))
	{
		return false;
	}

	for (auto& [_, descriptorSet] : mDescriptorSets)
	{
		Coral::DescriptorSetConfig config{};

		for (const auto& [binding, uniform] : descriptorSet.uniforms)
		{
			config.bindings.push_back({ binding, uniform.buffer.get() });
		}

		for (const auto& [binding, sampler] : descriptorSet.samplers)
		{

			config.bindings.push_back({ binding, sampler });
		}

		descriptorSet.descriptorSet = Coral::RAII::createDescriptorSet(&context, config);
	}

	return true;
}


bool
MaterialResource::updateUniformBlocks(const Reef::Material& material, Coral::Context& context)
{
	// Transfer the unstructured uniforms into the structured uniform buffers
	for (auto& [_, uniform] : mDescriptorSets
		| std::views::transform([](auto& set) -> auto& { return set.second.uniforms; })
		| std::views::join)
	{
		for (const auto& [i, member] : uniform.block.getBlockDefinition().members | std::views::enumerate)
		{
			material.getParameter(member.name)
				.transform([&](const auto& value)
					{
						return Common::visit(value, Common::Visitor{
							[&](float v) { return uniform.block.setScalar(i, v); },
							[&](int v) { return uniform.block.setScalar(i, v); },
							//[&](bool v) { uniformData.block.setValue(i, v); },
							[&](const glm::mat3& m) { return uniform.block.setMat33F(i, m); },
							[&](const glm::mat4& m) { return uniform.block.setMat44F(i, m); },
							[&](const glm::vec2& v) { return uniform.block.setVec2F(i, v); },
							[&](const glm::vec3& v) { return uniform.block.setVec3F(i, v); },
							[&](const glm::vec4& v) { return uniform.block.setVec4F(i, v); },
							[&](const auto& v) {  assert(false); return false; }
							});
					});
		}

		updateUniformBuffer(uniform.block, *uniform.buffer);
	}

	return true;
}


bool
MaterialResource::update(const Reef::Material& material, Coral::Context& context)
{
	if (!shouldUpdate(material.version()))
	{
		return true;
	}

	auto shaderImpl = static_cast<ShaderImpl*>(material.shader().get());

	std::array shaders{ shaderImpl->vertexShader(), shaderImpl->fragmentShader() };

	if (!mPipelineState)
	{
		if (!createDescriptorSets(material, context))
		{
			return false;
		}

		Coral::PipelineStateConfig config{};
		config.shaderModules	    = shaders;
		config.framebufferSignature = { { Coral::PixelFormat::RGBA8_SRGB }, Coral::PixelFormat::DEPTH24_STENCIL8 };
		mPipelineState			    = Coral::RAII::createPipelineState(&context, config);
	}

	if (!mPipelineState)
	{
		return false;
	}

	return updateUniformBlocks(material, context);
}


bool
MaterialResource::bind(Coral::CommandBuffer& commandBuffer)
{
	commandBuffer.cmdBindPipeline(mPipelineState.get());

	for (const auto& [index, descriptorSet] : mDescriptorSets)
	{
		commandBuffer.cmdBindDescriptorSet(descriptorSet.descriptorSet.get(), index);
	}
	return false;
}