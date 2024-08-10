module;

#include <Engine/System.h>

#include <memory>
#include <vector>
#include <string>
#include <string_view>

export module Engine.GameObject;

export import Engine.Transform;

import Engine.Component;
import Engine.SubclassOf;
import Engine.Object;
import Engine.Time;
import Common.SmartPointer;

export namespace Reef
{
class Application;

class Scene;

class ENGINE_API GameObject final: public SubclassOf<GameObject, Object>
{
public:

	GameObject();

	~GameObject();

	GameObject* parent();

	const GameObject* parent() const;

	void setParent(GameObject* parent, bool preserveWorldPose = true);

	std::vector<GameObject*> children();

	std::vector<const GameObject*> children() const;

	const std::string& name() const;

	void setName(std::string_view name);

	Transform& transform();

	const Transform& transform() const;

	Scene& scene();

	const Scene& scene() const;

	void setScene(Scene& scene);

	Component* addComponent(const Type& type, bool autoActivate = true);

	template<typename T>
	T* addComponent(bool autoActivate = true);

	template<typename T>
	std::vector<T*> getComponents();

	template<typename T>
	std::vector<const T*> getComponents() const;

	template<typename T>
	T* getComponent();

	const Time& time() const;

	const Application& application() const;

	Application& application();

	bool isBeingDestroyed() const;

private:

	friend class Scene;

	std::string mName;

	Transform mTransform;

	Scene* mScene{ nullptr };

	GameObject* mParent{ nullptr };

	std::vector<GameObject*> mChildren;

	std::vector<std::unique_ptr<Component>> mComponents;

	bool mIsBeingDestroyed{ false };
};

} // export namespace Reef


template<typename T>
T*
Reef::GameObject::addComponent(bool autoActivate)
{
	return static_cast<T*>(addComponent(T::classType(), autoActivate));
}


template<typename T>
std::vector<T*> 
Reef::GameObject::getComponents()
{
	const auto& type = T::classType();
	std::vector<T*> components;
	for (auto& component : mComponents)
	{
		if (component->isOfType(type))
		{
			components.push_back(static_cast<T*>(component.get()));
		}
	}

	return components;
}


template<typename T>
std::vector<const T*> 
Reef::GameObject::getComponents() const
{
	auto type = T::classType();
	std::vector<const T*> components;
	for (auto& component : mComponents)
	{
		if (component->isOfType(type))
		{
			components.push_back(static_cast<const T*>(component.get()));
		}
	}

	return components;
}


template<typename T>
T*
Reef::GameObject::getComponent()
{
	const auto& type = T::classType();

	for (auto& component : mComponents)
	{
		if (component->isOfType(type))
		{
			return static_cast<T*>(component.get());
		}
	}
	return nullptr;
}