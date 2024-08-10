module;

#include <chrono>
#include <mutex>
#include <string>
#include <vector>
#include <memory>

module Engine.SceneManager;

using namespace Reef;

import Engine.Application;


SceneManager::SceneManager(IApplication& application)
	: mApplication(application)
	, mActiveScene(std::make_unique<Scene>())
{
	
}


Scene*
SceneManager::activeScene()
{
	return mActiveScene.get();
}