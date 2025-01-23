#include <Engine/Core/SceneManager.hpp>

#include <Engine/Application/Application.hpp>

#include <Engine/Core/Scene.hpp>

#include <Common/SmartPointer.hpp>


using namespace Reef;


SceneManager::SceneManager(Application& application)
	: mApplication(application)
{
	if (auto ctor = TypeRegistry::getTypeTraits(Scene::classType()).constructorDelegate)
	{
		auto scene = ctor();
		mActiveScene = Common::staticPointerCast<Scene>(std::move(scene));
		mActiveScene->initialize(application);
	}
}


SceneManager::~SceneManager()
{}


Scene*
SceneManager::activeScene()
{
	return mActiveScene.get();
}