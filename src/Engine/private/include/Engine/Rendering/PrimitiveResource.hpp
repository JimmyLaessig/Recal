#ifndef ENGINE_RENDERING_PRIMITIVERESOURCE_HPP
#define ENGINE_RENDERING_PRIMITIVERESOURCE_HPP

#include <memory>

#include <Engine/Rendering/RenderResource.hpp>

//import Engine.Rendering.RenderResource;

#include <Engine/Core/Primitive.hpp>

#include <Coral/RAII.hpp>

namespace Reef::Rendering
{

class ResourceManager;

class PrimitiveResource : public RenderResource
{
public:
	
	PrimitiveResource(ResourceManager& resourceManager);

	Coral::BufferView* indices();

	Coral::BufferView* positions();

	Coral::BufferView* normals();

	Coral::BufferView* tangents();

	Coral::BufferView* texcoords();

	uint32_t indexCount();

	void bind(Coral::CommandBuffer& commandBuffer);

	bool update(const Reef::Primitive& primitive, Coral::Context& context);

private:

	Coral::RAII::BufferPtr mIndexBuffer{ nullptr };
	Coral::RAII::BufferPtr mPositionBuffer{ nullptr };
	Coral::RAII::BufferPtr mNormalBuffer{ nullptr };
	Coral::RAII::BufferPtr mTangentBuffer{ nullptr };
	Coral::RAII::BufferPtr mTexcoordBuffer{ nullptr };

	Coral::RAII::BufferViewPtr mIndices{ nullptr };
	Coral::RAII::BufferViewPtr mPositions{ nullptr };
	Coral::RAII::BufferViewPtr mNormals{ nullptr };
	Coral::RAII::BufferViewPtr mTangents{ nullptr };
	Coral::RAII::BufferViewPtr mTexcoords{ nullptr };

	uint32_t mIndexCount{ 0 };
};

} // Reef::Rendering 

#endif //! ENGINE_RENDERING_PRIMITIVERESOURCE_HPP
