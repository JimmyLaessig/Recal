module;

#include <Engine/System.h>

#include <memory>
#include <mutex>
#include <string_view>
#include <vector>

export module Engine.Scene;

import Engine.Time;

import Engine.Object;
import Engine.Component;
import Engine.SubclassOf;
import Engine.GameObject;
import Common.Event;

export namespace Reef
{

class Application;

class ENGINE_API Scene : public SubclassOf<Scene, Object>
{
public:

	GameObject* createGameObject(std::string_view name = "GameObject");

	Component* addComponent(const Type& type, GameObject& owner, bool activate = true);

	std::vector<GameObject*> gameObjects();

	template<typename T>
	T* findComponent();

	template<typename T>
	std::vector<T*> findComponents();

	/// Start the scene
	void start();

	/// Stop the scene
	void stop();

	/// Update the scene
	void update();

	const Time& time() const;

	const Application& application() const;

	Application& application();

	//const Common::Event<>& onUpdateFrameEvent() const { return mOnUpdateFrameEvent; }
	//const Common::Event<>& onStartEvent() const { return mOnStartEvent; }
	//const Common::Event<>& onStopEvent() const { return mOnStopEvent; }
	//const Common::Event<>& onInitializeEvent() const { return mOnInitializeEvent; }

	bool isRunning() const;

	//void destroy(Component* component);

	//void destroy(GameObject* gameObject);

	//void runGarbageCollection();

private:

	//void destroyNow(GameObject* gameObject);

	void updateTime();

	Application* mApplication{ nullptr };

	Timepoint mStartTimestamp{ 0 };

	Time mTime;

	bool mIsRunning{ false };

	//bool mRunGarbageCollection{ false };

	std::mutex mGameObjectsProtection;

	std::vector<std::unique_ptr<GameObject>> mGameObjects;

	//std::vector<std::unique_ptr<Object>> mDestructionQueue;
};


template<typename T>
T*
Scene::findComponent()
{
	for (auto& go : mGameObjects)
	{
		if (auto comp = go->getComponent<T>())
		{
			return comp;
		}
	}

	return nullptr;
}


template<typename T>
std::vector<T*>
Scene::findComponents()
{
	std::vector<T*> res;
	for (auto& go : mGameObjects)
	{
		if (auto comp = go->getComponent<T>())
		{
			res.push_back(comp);
		}
	}

	return res;
}

} // export namespace Reef