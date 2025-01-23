#ifndef ENGINE_CORE_SCENE_HPP
#define ENGINE_CORE_SCENE_HPP

#include <Engine/Core/ECSBase.hpp>
#include <Engine/Core/SceneObject.hpp>
#include <Engine/Core/SubclassOf.hpp>
#include <Engine/Core/Component.hpp>

#include <Engine/Rendering/SceneView.hpp>

#include <memory>
#include <mutex>
#include <string_view>
#include <vector>

#include <ranges>

namespace Reef
{

class ENGINE_API Scene : public SubclassOf<Scene, ECSBase>
{
public:

	SceneObject* createSceneObject(std::string_view name = "SceneObject");

	std::vector<SceneObject*> gameObjects();

	/// Start the scene
	void start();

	/// Stop the scene
	void stop();

	/// Update the scene
	void update();

	//const Time& time() const;

	//const Common::Event<>& onUpdateFrameEvent() const { return mOnUpdateFrameEvent; }
	//const Common::Event<>& onStartEvent() const { return mOnStartEvent; }
	//const Common::Event<>& onStopEvent() const { return mOnStopEvent; }
	//const Common::Event<>& onInitializeEvent() const { return mOnInitializeEvent; }

	bool isRunning() const;

	//void destroy(Component* component);

	//void destroy(GameObject* gameObject);

	//void runGarbageCollection();

	Reef::Rendering::SceneView createSceneView();

	template<ComponentType T>
	T* findComponent();

	Component* findComponent(const Type& type);

	template<ComponentType T>
	std::vector<T*> getComponents();

	std::vector<Component*> getComponents(const Type& type);

private:

	//void destroyNow(GameObject* gameObject);

	void updateTime();

	//Application& mApplication;

	//Timepoint mStartTimestamp{ 0 };

	//Time mTime;

	bool mIsRunning{ false };

	//bool mRunGarbageCollection{ false };

	std::mutex mGameObjectsProtection;

	std::vector<std::unique_ptr<SceneObject>> mGameObjects;

	//std::vector<std::unique_ptr<Object>> mDestructionQueue;
};

template<ComponentType T>
T*
Scene::findComponent()
{
	return static_cast<T*>(findComponent(T::classType()));
}


template<ComponentType T>
std::vector<T*>
Scene::getComponents()
{
	return getComponents(T::classType())
		| std::views::transform([](Component* c) { return static_cast<T*>(c); })
		| std::ranges::to<std::vector>();
}

} // namespace Reef

#endif //!ENGINE_CORE_SCENE_HPP