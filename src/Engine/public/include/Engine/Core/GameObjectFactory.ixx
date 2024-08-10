module;

#include <memory>

#include <tiny_gltf.h>

export module Engine.GameObjectFactory;

import Engine.GameObject;
import Engine.Scene;
//import Engine.Texture;

namespace Reef
{

export class GameObjectFactory
{
public:

	static GameObject* createGameObject(Scene& scene, const tinygltf::Model& model);

	//static GameObject* createSkybox(Scene& scene, std::shared_ptr<Engine::Texture> skyboxTexture);

	//static GameObject* createCoordinateCross(Scene& scene, float scale = 0.5f);
};

} // namespace Reef
