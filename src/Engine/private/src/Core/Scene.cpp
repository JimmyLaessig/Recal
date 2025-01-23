#include <Engine/Core/Scene.hpp>
#include <Engine/Core/SceneObject.hpp>
#include <Engine/Application/Application.hpp>
#include <Common/SmartPointer.hpp>

#include <Engine/Component/CameraComponent.hpp>
#include <Engine/Component/MeshComponent.hpp>

#include <glm/glm.hpp>

#include <chrono>
#include <mutex>
#include <string>
#include <vector>

using namespace Reef;


SceneObject*
Scene::createSceneObject(std::string_view name)
{
	std::lock_guard lock(mGameObjectsProtection);

	if (auto ctor = TypeRegistry::getTypeTraits(SceneObject::classType()).constructorDelegate)
	{
		auto go = Common::staticPointerCast<SceneObject>(ctor());
		go->setName(name);
		go->setScene(*this);
		go->initialize(application());
		return mGameObjects.emplace_back(std::move(go)).get();
	}
	{
		return nullptr;
	}
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


std::vector<SceneObject*>
Scene::gameObjects()
{
	std::vector<SceneObject*> result;

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
	//if (mIsRunning)
	//{
	//	// TODO: Log error that scene is already running
	//	return;
	//}

	//mStartTimestamp				= mTime.now();
	//mTime.currentFrameTimestamp = mStartTimestamp;
	//mTime.deltaTime				= 0;

	// TODO: Invoke initialize callbacks

	// TODO: Invoke start callbacks
}


void
Scene::stop()
{
//	if (!mIsRunning)
//	{
//		// TODO: Log error that scene is not running
//		return;
//	}
//
//	mIsRunning = false;
//
//	auto now = mTime.now();
//	mTime.currentFrameTimestamp = mStartTimestamp;
//	mTime.deltaTime = 0;

	// TODO: Invoke stop callbacks
}


void
Scene::updateTime()
{
	/*auto now = mTime.now();
	mTime.currentFrameTimestamp = mStartTimestamp;
	
	double deltaTime = static_cast<double>(now - mTime.currentFrameTimestamp) * Constants<double>::NANOSECONDS_TO_SECONDS;
	mTime.deltaTime = static_cast<float>(deltaTime);*/
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

//const
//Time& Scene::time() const
//{
//	return mTime;
//}


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


Reef::Rendering::SceneView
Scene::createSceneView()
{
	Reef::Rendering::SceneView view{};

	if (auto cameraComponent = findComponent<CameraComponent>())
	{
		auto cameraModelMatrix			 = cameraComponent->transform().localToWorldMatrix();
		view.camera						 = cameraComponent->camera();
		view.viewMatrix					 = glm::inverse(cameraModelMatrix);
		view.viewProjectionMatrix	     = view.camera.projectionMatrix * view.viewMatrix;
		view.inverseViewProjectionMatrix = glm::inverse(view.viewProjectionMatrix);
	}

	for (auto meshComponent : getComponents<MeshComponent>())
	{
		Reef::Rendering::RenderObject ro;
		ro.transform.localToWorldMatrix = meshComponent->transform().localToWorldMatrix();
		ro.transform.worldToLocalMatrix = meshComponent->transform().worldToLocalMatrix();
		ro.transform.normalMatrix		= glm::mat3(glm::inverse(glm::transpose(ro.transform.localToWorldMatrix)));
		ro.transform.objectId			= reinterpret_cast<size_t>(&meshComponent->owner());
		
		auto mesh = meshComponent->mesh();
		for (size_t i = 0; i < mesh->getNumPrimitives(); ++i)
		{
			ro.primitive = mesh->getPrimitive(i);
			ro.material = meshComponent->material(i);
			view.renderObjects.push_back(ro);
		}
	}

	return view;
}


Component*
Scene::findComponent(const Type& type)
{
	for (auto& go : mGameObjects)
	{
		if (auto comp = go->getComponent(type))
		{
			return comp;
		}
	}

	return nullptr;
}


std::vector<Component*>
Scene::getComponents(const Type& type)
{
	std::vector<Component*> components;

	for (auto& go : mGameObjects)
	{
		go->getComponents(type, components);
	}
	return components;
}