module;

#include <cstdint>
#include <memory>

#include <unordered_map>
#include <variant>

export module Engine.Rendering.ResourceManager;

//import Engine.Component.MeshComponent;
//
//import Engine.Primitive;
//import Engine.Texture;
//import Engine.Shader;
//import Engine.Material;
//
//import Engine.Rendering.PrimitiveResource;
//import Engine.Rendering.TextureResource;
//import Engine.Rendering.SceneView;

//import RenderLib;

namespace Reef
{

namespace Rendering
{

export class ResourceManager
{
public:

	//bool initialize(RenderLib::Context& context);

	//TextureResource* updateResource(const Engine::Texture& texture);

	//PrimitiveResource* updateResource(const Engine::Primitive& primitive);

	//PrimitiveResource* getResource(const Engine::Primitive& primitive);

	//TextureResource* getResource(const Engine::Texture& texture);

	//RenderLib::Context* context() { return mContext; }

private:

	//RenderLib::Context* mContext{ nullptr };

	//using Resource = std::variant<std::unique_ptr<PrimitiveResource>, 
	//							  std::unique_ptr<TextureResource>>;

	//std::unordered_map<size_t, Resource> mResources;

	//std::unordered_map<size_t, size_t> mLastUpd;
};

} // namespace Rendering

} // namespace Reef
