#ifndef ENGINE_RENDERING_MATERIALRESOURCE_HPP
#define ENGINE_RENDERING_MATERIALRESOURCE_HPP

#include <Engine/Rendering/RenderResource.hpp>

#include <memory>

#include <Engine/Core/Material.hpp>

#include <Coral/UniformBlockBuilder.hpp>
#include <Coral/RAII.hpp>

namespace Reef::Rendering
{


class ENGINE_API MaterialResource : public RenderResource
{
public:

	MaterialResource(ResourceManager& resourceManager);

	/// Update the material's render resource
	bool update(const Reef::Material& material, Coral::Context& context);

	/// Bind the material resource to the command buffer
	bool bind(Coral::CommandBuffer& commandBuffer);

private:

	bool createUniformBlocks(const Reef::Material& material, Coral::Context& context);

	//bool createUniformSamplers(const Reef::Material& material, Coral::Context& context);

	bool createDescriptorSets(const Reef::Material& material, Coral::Context& context);

	bool updateUniformBlocks(const Reef::Material& material, Coral::Context& context);

	struct UniformBufferData
	{
		Coral::RAII::BufferPtr buffer;
		Coral::UniformBlockBuilder block;
	};

	struct DescriptorSetData
	{
		std::unordered_map<uint32_t, UniformBufferData> uniforms;
		std::unordered_map<uint32_t, Coral::Sampler*> samplers;
		Coral::RAII::DescriptorSetPtr descriptorSet;
	};

	/// The descriptor sets of the materials
	std::unordered_map<uint32_t, DescriptorSetData> mDescriptorSets;

	/// The pipeline state of the materials
	Coral::RAII::PipelineStatePtr mPipelineState;
};

} // namespace Reef::Rendering

#endif //!ENGINE_RENDERING_MATERIALRESOURCE_HPP
