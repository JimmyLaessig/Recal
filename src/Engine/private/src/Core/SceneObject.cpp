#include <Engine/Core/SceneObject.hpp>

#include <Engine/Core/Scene.hpp>
#include <Engine/Core/Component.hpp>
#include <Engine/Core/TypeRegistry.hpp>

#include <Common/SmartPointer.hpp>

#include <algorithm>
#include <vector>
#include <memory>
#include <string>

using namespace Reef;


SceneObject::SceneObject()
	: mTransform(*this)
{
}


SceneObject::~SceneObject()
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


SceneObject*
SceneObject::parent()
{
	return mParent;
}


const SceneObject*
SceneObject::parent() const
{
	return mParent;
}


void
SceneObject::setParent(SceneObject* parent, bool preserveWorldPose)
{
	/*auto position = mTransform.worldPosition();
	auto rotation = mTransform.worldRotation();
	auto scale = mTransform.worldScale();*/

	if (mParent)
	{
		std::erase(mParent->mChildren, this);
	}

	mParent = parent;

	if (mParent)
	{
		mParent->mChildren.push_back(this);
	}

	if (preserveWorldPose)
	{
		//mTransform.setWorldPose(position, rotation, scale);
	}
}


std::vector<SceneObject*> 
SceneObject::children()
{
	return mChildren;
}


std::vector<const SceneObject*>
SceneObject::children() const
{
	std::vector<const SceneObject*> result;
	for (auto child : mChildren)
	{
		result.push_back(child);
	}
	return result;
}


const std::string&
SceneObject::name() const
{
	return mName;
}


void
SceneObject::setName(std::string_view name)
{
	mName.assign(name.data(), name.size());
}


Transform&
SceneObject::transform()
{
	return mTransform;
}


const Transform&
SceneObject::transform() const
{
	return mTransform;
}


Scene&
SceneObject::scene()
{
	return *mScene;
}


const Scene&
SceneObject::scene() const
{
	return *mScene;
}


void
SceneObject::setScene(Scene& scene)
{
	mScene = &scene;
}


Component*
SceneObject::addComponent(const Type& type, bool activate)
{ 
	auto traits = TypeRegistry::getTypeTraits(type);
	if (auto ctor = TypeRegistry::getTypeTraits(type).constructorDelegate)
	{
		auto comp = Common::staticPointerCast<Component>(ctor());
		auto ptr = comp.get();

		mComponents.push_back(std::move(comp));

		ptr->setOwner(*this);
		ptr->initialize(application());
		ptr->onComponentCreated();
		ptr->setActive(activate);

		return ptr;
	}
	else
	{
		return nullptr;
	}
}


Component*
SceneObject::getComponent(const Type& type)
{
	auto iter = std::ranges::find_if(mComponents, [&](const auto& c) { return c->type() == type; });
	return iter != mComponents.end() ? iter->get() : nullptr;
}


const Component*
SceneObject::getComponent(const Type& type) const
{
	auto iter = std::ranges::find_if(mComponents, [&](const auto& c) { return c->type() == type; });
	return iter != mComponents.end() ? iter->get() : nullptr;
}


std::vector<Component*>
SceneObject::getComponents(const Type& type)
{
	std::vector<Component*> result;
	getComponents(type, result);
	return result;
}


std::vector<const Component*>
SceneObject::getComponents(const Type& type) const
{
	std::vector<const Component*> result;
	getComponents(type, result);
	return result;
}


/// Find all components of type T and append them to the provided vector
void
SceneObject::getComponents(const Type& type, std::vector<Component*>& components)
{
	auto c = mComponents
		| std::views::filter([&](const auto& c) { return c->type() == type; })
		| std::views::transform([&](const auto& c) -> Component* { return c.get(); });

	components.append_range(c);
}

/// Find all components of type T and append them to the provided vector
void
SceneObject::getComponents(const Type& type, std::vector<const Component*>& components) const
{
	auto c = mComponents
		| std::views::filter([&](const auto& c) { return c->type() == type; })
		| std::views::transform([&](const auto& c) -> const Component* { return c.get(); });

	components.append_range(c);
}


std::vector<Component*>
SceneObject::getComponents()
{
	return mComponents
		| std::views::transform([&](const auto & c) { return c.get(); })
		| std::ranges::to<std::vector>();
}


std::vector<const Component*>
SceneObject::getComponents() const
{
	return mComponents
		| std::views::transform([&](const auto& c) -> const Component* { return c.get(); })
		| std::ranges::to<std::vector>();
}


void
SceneObject::getComponents(std::vector<Component*>& components)
{
	auto range = mComponents | std::views::transform([&](const auto& c) -> Component* { return c.get(); });
	components.append_range(range);
}


void 
SceneObject::getComponents(std::vector<const Component*>& components) const
{
	auto range = mComponents | std::views::transform([&](const auto& c) -> const Component* { return c.get(); });
	components.append_range(range);
}
