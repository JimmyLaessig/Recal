module;

#include <algorithm>
#include <vector>
#include <memory>
#include <string>

module Engine.GameObject;

import Engine.Scene;
import Engine.TypeRegistry;

using namespace Reef;


GameObject::GameObject()
	: mTransform(*this)
{
}


GameObject::~GameObject()
{
	for (auto& component : mComponents)
	{
		if (component)
		{
			component->onComponentDestroyed();
			component.reset();
		}
	}
}


GameObject*
GameObject::parent()
{
	return mParent;
}


const GameObject*
GameObject::parent() const
{
	return mParent;
}


void
GameObject::setParent(GameObject* parent, bool preserveWorldPose)
{
	/*auto position = mTransform.worldPosition();
	auto rotation = mTransform.worldRotation();
	auto scale = mTransform.worldScale();*/

	mParent = parent;
	mParent->mChildren.push_back(this);

	if (preserveWorldPose)
	{
		//mTransform.setWorldPose(position, rotation, scale);
	}
}


std::vector<GameObject*> 
GameObject::children()
{
	return mChildren;
}


std::vector<const GameObject*>
GameObject::children() const
{
	std::vector<const GameObject*> result;
	for (auto child : mChildren)
	{
		result.push_back(child);
	}
	return result;
}


const std::string&
GameObject::name() const
{
	return mName;
}


void
GameObject::setName(std::string_view name)
{
	mName.assign(name.data(), name.size());
}


Transform&
GameObject::transform()
{
	return mTransform;
}


const Transform&
GameObject::transform() const
{
	return mTransform;
}


Scene&
GameObject::scene()
{
	return *mScene;
}


const Scene&
GameObject::scene() const
{
	return *mScene;
}


void
GameObject::setScene(Scene& scene)
{
	mScene = &scene;
}


Component*
GameObject::addComponent(const Type& type, bool activate)
{
	return mScene->addComponent(type, *this, activate);
}

//std::vector<Component*>
//GameObject::getComponents()
//{
//	std::vector<Component*> result(mComponents.size(), nullptr);
//	std::transform(mComponents.begin(), mComponents.end(), result.begin(),
//				   [](auto& component) { return component.get(); });
//
//	return result;
//}


//std::vector<const Component*>
//GameObject::getComponents() const
//{
//	std::vector<const Component*> result(mComponents.size(), nullptr);
//	std::transform(mComponents.begin(), mComponents.end(), result.begin(),
//				   [](const auto& component) { return component.get(); });
//
//	return result;
//}


//void 
//GameObject::onUpdate(float deltaTime)
//{
//	for (auto& comp : mComponents)
//	{
//		comp->onUpdate(deltaTime);
//	}
//}


//void
//Engine::destroy(Component* component)
//{
//	if (!component)
//	{
//		return;
//	}
//
//	auto& gameObject = component->gameObject();
//	// Remove the Component from the GameObject's list. This will will eventually invoke the component's constructor
//	std::erase_if(gameObject.mComponents, [component](auto& comp) { return comp.get() == component; });
//}


//GameObject* 
//Engine::createGameObject()
//{
//	if (auto scene = Scene::currentScene())
//	{
//		return scene->createGameObject();
//	}
//
//	// TODO: LOG error here
//	return nullptr;
//}


//void
//Engine::destroy(GameObject* gameObject)
//{
//	if (!gameObject)
//	{
//		return;
//	}
//
//	if (auto scene = gameObject->mScene)
//	{
//		scene->destroyGameObject(gameObject);
//	}
//	else
//	{
//		// TODO: LOG error here
//	}
//}


const
Time& GameObject::time() const
{
	return mScene->time();
}


const
Application&
GameObject::application() const
{
	return mScene->application();
}


Application&
GameObject::application()
{
	return mScene->application();
}


bool
GameObject::isBeingDestroyed() const
{
	return mIsBeingDestroyed;
}