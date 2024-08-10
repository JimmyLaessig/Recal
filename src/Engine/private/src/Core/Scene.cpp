module;

#include <chrono>
#include <mutex>
#include <string>
#include <vector>

module Engine.Scene;

using namespace Reef;

import Engine.Constants;
import Engine.TypeRegistry;
import Common.SmartPointer;

GameObject*
Scene::createGameObject(std::string_view name)
{
	std::lock_guard lock(mGameObjectsProtection);
	mGameObjects.push_back(std::unique_ptr<GameObject>(new GameObject));
	auto go = mGameObjects.back().get();
	go->mScene = this;

	go->setName(name);
	
	return go;
}


Component*
Scene::addComponent(const Type& type, GameObject& owner, bool activate)
{
	if (!type.isSubclassOf(Component::classType()))
	{
		// TODO: Log error
		return nullptr;
	}

	auto ctor = TypeRegistry::constructor(type);

	if (!ctor)
	{
		// TODO: Log error
		return nullptr;
	}

	auto obj = ctor();

	auto component = Common::staticPointerCast<Component>(std::move(obj));

	if (!component)
	{
		return nullptr;
	}

	auto ptr = owner.mComponents.emplace_back(std::move(component)).get();

	ptr->mOwner = &owner;

	if (auto updateFun = TypeRegistry::updateFunction(type))
	{
		updateFun(*ptr);
	}
	//auto updateFun = ptr->componentUpdateFunction();

	ptr->onComponentCreated();

	if (isRunning())
	{
		if (activate)
		{
			ptr->setActive(true);
		}

		ptr->start();
	}

	return ptr;
}


//void
//Scene::destroyNow(Engine::GameObject* gameObject)
//{
//	auto iter = std::find_if(mGameObjects.begin(), mGameObjects.end(), [=](auto& go)
//	{
//		return go.get() == gameObject;
//	});
//
//	if (iter == mGameObjects.end())
//	{
//		// TODO: Log error
//		return;
//	}
//
//	// Obtain temporary ownership over the GameObject before it is finally destroyed
//	std::unique_ptr<GameObject> go;
//	std::swap(go, *iter);
//}

//void
//Scene::destroy(GameObject* gameObject)
//{
//	auto iter = std::find_if(mGameObjects.begin(), mGameObjects.end(), [=](auto& go)
//	{
//		return go.get() == gameObject;
//	});
//
//	if (iter == mGameObjects.end())
//	{
//		// TODO: Log error
//		return;
//	}
//
//	// Mark the GameObject and it's Component to be destroyed
//	gameObject->mIsBeingDestroyed = true;
//	for (auto& component : gameObject->mComponents)
//	{
//		component->mIsBeingDestroyed = true;
//	}
//
//	mRunGarbageCollection = true;
//
//	// Unparent the GameObject right away
//	auto parent = gameObject->parent();
//	gameObject->setParent(nullptr);
//
//	// Reparent the children
//	for (auto& child : gameObject->mChildren)
//	{
//		child->setParent(parent);
//	}
//}


//void
//Scene::destroy(Component* component)
//{
//	component->mIsBeingDestroyed = true;
//	mRunGarbageCollection = true;
//}


std::vector<GameObject*> 
Scene::gameObjects()
{
	std::vector<GameObject*> result;

	for (auto& go : mGameObjects)
	{
		if (!go->parent())
		{
			result.push_back(go.get());
		}
	}

	return result;
}


void
Scene::start()
{
	if (mIsRunning)
	{
		// TODO: Log error that scene is already running
		return;
	}

	mStartTimestamp				= mTime.now();
	mTime.currentFrameTimestamp = mStartTimestamp;
	mTime.deltaTime				= 0;

	// TODO: Invoke initialize callbacks

	// TODO: Invoke start callbacks
}


void
Scene::stop()
{
	if (!mIsRunning)
	{
		// TODO: Log error that scene is not running
		return;
	}

	mIsRunning = false;

	auto now = mTime.now();
	mTime.currentFrameTimestamp = mStartTimestamp;
	mTime.deltaTime = 0;

	// TODO: Invoke stop callbacks
}


void
Scene::updateTime()
{
	auto now = mTime.now();
	mTime.currentFrameTimestamp = mStartTimestamp;
	
	double deltaTime = static_cast<double>(now - mTime.currentFrameTimestamp) * Constants<double>::NANOSECONDS_TO_SECONDS;
	mTime.deltaTime = static_cast<float>(deltaTime);
}


void
Scene::update()
{
	if (!mIsRunning)
	{
		// TODO: Log error that scene is not running
		return;
	}

	updateTime();

	//mOnUpdateFrameEvent.broadcast();
}


bool
Scene::isRunning() const
{
	return mIsRunning;
}


const Application&
Scene::application() const
{
	return *mApplication;
}


Application&
Scene::application()
{
	return *mApplication;
}


const
Time& Scene::time() const
{
	return mTime;
}


//void
//Scene::runGarbageCollection()
//{
//	/*mRunGarbageCollection = false;
//
//	std::mutex mGameObjectsProtection;
//	mGameObjects.erase_if([](auto& gameObject) { return gameObject->isBeingDestroyed(); });*/
//
//	//mGameObjects
//}