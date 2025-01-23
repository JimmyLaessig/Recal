#ifndef ENGINE_CORE_SCENEMANAGER_HPP
#define ENGINE_CORE_SCENEMANAGER_HPP

#include <Engine/System.hpp>


#include <memory>

namespace Reef
{
class Scene;
class Application;

class ENGINE_API SceneManager
{
public:

	~SceneManager();

	SceneManager(Application& application);

	Scene* activeScene();

private:

	std::unique_ptr<Scene> mActiveScene;

	Application& mApplication;

};

} // namespace Reef


#endif // !ENGINE_CORE_SCENEMANAGER_HPP