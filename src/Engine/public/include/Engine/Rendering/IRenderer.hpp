#ifndef ENGINE_RENDERING_IRENDERER_HPP
#define ENGINE_RENDERING_IRENDERER_HPP

#include <Engine/System.hpp>

#include <Engine/Rendering/Shader.hpp>

#include <Engine/Rendering/SceneView.hpp>

namespace Reef::Rendering
{

namespace BuiltInShaders
{

constexpr auto DefaultPBR = "DefaultPBR";

} // namespace BuiltInShaders

class ENGINE_API IRenderer
{
public:

	~IRenderer() = default;

	virtual void render(SceneView&& sceneView) = 0;

	virtual std::shared_ptr<Shader> getShader(std::string_view name) = 0;
};

} // namespace Reef::Rendering

#endif // !ENGINE_RENDERING_IRENDERER_HPP
