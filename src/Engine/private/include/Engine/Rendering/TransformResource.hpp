#ifndef ENGINE_RENDERING_TRANSFORMRESOURCE_HPP
#define ENGINE_RENDERING_TRANSFORMRESOURCE_HPP


#include <Engine/Rendering/RenderResource.hpp>

#include <Engine/Rendering/SceneView.hpp>

#include <Engine/Core/Transform.hpp>

#include <Coral/RAII.hpp>

#include <Coral/UniformBlockBuilder.hpp>


namespace Reef::Rendering
{

class ENGINE_API TransformResource : public RenderResource
{
public:

	TransformResource(ResourceManager& resourceManager);

	Coral::DescriptorSet* getDescriptorSet();

	bool update(const TransformData& transform, Coral::Context& context);

	bool bind(Coral::CommandBuffer& commandBuffer);

private:

	Coral::UniformBlockBuilder mBuilder{ Coral::UniformBlockDefinition{ {
		Coral::MemberDefinition{ Coral::ValueType::MAT44F, "modelMatrix", 1 },
		Coral::MemberDefinition{ Coral::ValueType::MAT44F, "inverseModelMatrix", 1 },
		Coral::MemberDefinition{ Coral::ValueType::MAT33F, "normalMatrix", 1 },
	} } };

	Coral::RAII::BufferPtr mUniformBuffer{ nullptr };

	Coral::RAII::DescriptorSetPtr mDescriptorSet{ nullptr };
};

} // Reef::Rendering 

#endif //! ENGINE_RENDERING_INSTANCEDATARESOURCE_HPP
