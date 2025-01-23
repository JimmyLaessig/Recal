
#ifndef ENGINE_RENDERING_RESOURCEMANAGER_H
#define ENGINE_RENDERING_RESOURCEMANAGER_H

#include <Engine/Rendering/ResourceManager.hpp>

#include <Engine/Rendering/PrimitiveResource.hpp>
#include <Engine/Rendering/MaterialResource.hpp>
#include <Engine/Rendering/TransformResource.hpp>
#include <Engine/Rendering/TextureResource.hpp>

#include <Engine/Rendering/SceneView.hpp>

#include <Coral/RAII.hpp>

#include <cstdint>
#include <memory>
#include <unordered_map>
#include <variant>


namespace Reef::Rendering
{

//class ENGINE_API RenderObjectResolved
//{
//public:
//
//	bool recordRenderCommand(Coral::CommandBuffer& commandBuffer);
//
//	private:
//
//	std::shared_ptr<Coral::PipelineState> mPipelineState;
//
//	std::shared_ptr<Coral::DescriptorSet> mDescriptorSet;
//
//	struct BufferResource
//	{
//		std::shared_ptr<Coral::Buffer> buffer;
//		std::shared_ptr<Coral::BufferView> bufferView;
//	};
//
//	std::shared_ptr<Coral::ShaderModule> mVertexShader;
//
//	std::shared_ptr<Coral::ShaderModule> mFragmentShader;
//
//	BufferResource mIndices;
//
//	BufferResource mPositions;
//
//	std::shared_ptr<Coral::Buffer> mPerInstanceDataUniformBuffer;
//
//	std::shared_ptr<Coral::Buffer> mPerFrameDataUniformBuffer;
//
//	size_t id;
//};


//struct AttributeBufferResource
//{
//	Coral::Buffer* buffer{ nullptr };
//	Coral::BufferView* bufferView{ nullptr };
//};
//
//
//struct PrimitiveResource
//{
//	AttributeBufferResource positions;
//	Coral::Buffer* indices;
//};
//
//
//struct MaterialResource
//{
//	Coral::Buffer* perMaterialUniforms;
//	Coral::PipelineState* pipelineState;
//};
//
//
//struct InstanceData
//{
//
//};

//struct RenderObjectResource
//{
//	std::shared_ptr<>
//};


//struct OptimizedRenderObjectOrder
//{
//	std::unordered_map<MaterialResource*, std::unordered_map<PrimitiveResource*, std::vector<InstanceData>>> renderResources;
//};


struct RenderObjectResolved
{

};


class ENGINE_API ResourceManager
{
public:

	void initialize(Coral::Context& context);

	PrimitiveResource* getResource(const Reef::Primitive& primitive);

	MaterialResource* getResource(const Reef::Material& material);

	TransformResource* getResource(const TransformData& transform);

	TextureResource* getResource(const Reef::Texture& texture);

private:

	Coral::Context* mContext{ nullptr };

	using Resource = std::variant<std::unique_ptr<PrimitiveResource>,
								  std::unique_ptr<MaterialResource>,
								  std::unique_ptr<TransformResource>,
								  std::unique_ptr<TextureResource>>;

	std::unordered_map<size_t, Resource> mResources;
};

} // namespace Reef::Rendering

#endif // !ENGINE_RENDERING_RESOURCEMANAGER_H

