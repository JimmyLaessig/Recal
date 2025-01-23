#ifndef ENGINE_RENDERING_DEFAULTRENDERER_HPP
#define ENGINE_RENDERING_DEFAULTRENDERER_HPP

#include <Engine/Rendering/IRenderer.hpp>

#include <Engine/Rendering/ResourceManager.hpp>

#include <Coral/UniformBlockBuilder.hpp>
#include <glm/glm.hpp>

#include <memory>
#include <unordered_map>

namespace Reef
{
	class Application;
} // namespace Reef

namespace Reef::Rendering
{

class ENGINE_API DefaultRenderer : public IRenderer
{
public:

	bool initialize(Reef::Application& application);

	void render(SceneView&& sceneView) override;

	std::shared_ptr<Shader> getShader(std::string_view name) override;

private:

	struct RenderObjectResolved
	{
		PrimitiveResource* primitive{ nullptr };

		MaterialResource* materialResource{ nullptr };

		TransformResource* transformResouce{ nullptr };
	};

	struct SceneViewResolved
	{
		std::vector<RenderObjectResolved> renderObjects;
	};

	SceneViewResolved updateResources(const Reef::Rendering::SceneView& sceneView);

	bool initializeBuiltInShaders();

	Reef::Application* mApplication{ nullptr };

	Coral::RAII::ContextPtr mContext;

	Coral::RAII::SurfacePtr mSurface;

	Reef::Rendering::ResourceManager mResourceManager;

	Coral::UniformBlockBuilder mPerFrameUniforms{ Coral::UniformBlockDefinition{ { 
			Coral::MemberDefinition{ Coral::ValueType::MAT44F, "viewMatrix", 1 },
			Coral::MemberDefinition{ Coral::ValueType::MAT44F, "projectionMatrix", 1 },
			Coral::MemberDefinition{ Coral::ValueType::MAT44F, "viewProjectionMatrix", 1 },
	}}};

	Coral::RAII::BufferPtr mPerFrameUniformBuffer;

	Coral::RAII::DescriptorSetPtr mDescriptorSet;

	std::unordered_map<std::string, std::shared_ptr<Reef::Rendering::Shader>> mShaders;
};

} // namespace Reef

#endif // !ENGINE_RENDERING_DEFAULTRENDERER_HPP
