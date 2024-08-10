module;

#include <Engine/System.h>

#include <memory>

export module Engine.SceneManager;

import Engine.Scene;

export namespace Reef
{
class IApplication;

class ENGINE_API SceneManager
{
public:

	SceneManager(IApplication& application);

	Scene* activeScene();

private:

	std::unique_ptr<Scene> mActiveScene;

	IApplication& mApplication;

};

} // export namespace Reef