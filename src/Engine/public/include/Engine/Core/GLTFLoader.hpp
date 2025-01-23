#ifndef ENGINE_CORE_GLTFLOADER_HPP
#define ENGINE_CORE_GLTFLOADER_HPP

#include <Engine/Core/Scene.hpp>
#include <Engine/Core/SceneObject.hpp>

namespace Reef
{

class ENGINE_API GLTFLoader
{
public:

	static SceneObject* load(Scene& scene, const std::filesystem::path& path);

};

} // namespace Reef

#endif // !ENGINE_CORE_GLTFLOADER_HPP
