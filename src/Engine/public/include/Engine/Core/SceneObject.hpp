#ifndef ENGINE_CORE_SCENEOBJECT_HPP
#define ENGINE_CORE_SCENEOBJECT_HPP

#include <Engine/System.hpp>
#include <Engine/Core/Component.hpp>
#include <Engine/Core/Transform.hpp>

#include <memory>
#include <vector>
#include <string>
#include <string_view>
#include <concepts>
#include <ranges>


namespace Reef
{
class Scene;

template <typename T>
concept ComponentType = requires (T t) { std::derived_from <T, Component>; };

class ENGINE_API SceneObject : public SubclassOf<SceneObject, ECSBase>
{
public:

	SceneObject();

	~SceneObject();

	SceneObject* parent();

	const SceneObject* parent() const;

	void setParent(SceneObject* parent, bool preserveWorldPose = true);

	std::vector<SceneObject*> children();

	std::vector<const SceneObject*> children() const;

	const std::string& name() const;

	void setName(std::string_view name);

	Transform& transform();

	const Transform& transform() const;

	Scene& scene();

	const Scene& scene() const;

	void setScene(Scene& scene);

	/// Add a new component of the given type to the scene object
	/**
	 * \param type The type of the component to be created
	 * \param autoActivate Flag indicating if the component should be activated after construction
	 * \return Returns the newly created component
	 */
	Component* addComponent(const Type& type, bool autoActivate = true);

	/// Add a new component of the given type to the scene object
	/**
	 * \param T The type of the component to be created
	 * \param autoActivate Flag indicating if the component should be activated after construction
	 * \return Returns the newly created component
	 */
	template <ComponentType T>
	T* addComponent(bool autoActivate = true);

	/// Get the first component of the given type
	/**
	 * \return Returns a pointer to the component, if exists, otherwise nullptr.
	 */
	Component* getComponent(const Type& type);

	/// Find the first component of the given type
	/**
	 * \return Returns a pointer to the component, if exists, otherwise nullptr.
	 */
	const Component* getComponent(const Type& type) const;

	/// Get the first component of the given type
	/**
	 * \return Returns a pointer to the component, if exists, otherwise nullptr.
	 */
	template <ComponentType T>
	T* getComponent();

	/// Get the first component of the given type
	/**
	 * \return Returns a pointer to the component, if exists, otherwise nullptr.
	 */
	template <ComponentType T>
	const T* getComponent() const;

	/// Get all components of the given type
	std::vector<Component*> getComponents(const Type& type);

	/// Get all components of the given type
	std::vector<const Component*> getComponents(const Type& type) const;

	/// Get all components of type T
	template <ComponentType T>
	std::vector<T*> getComponents();

	/// Get all components of type T
	template <ComponentType T>
	std::vector<const T*> getComponents() const;

	/// Get all components of type T into the provided vector
	void getComponents(const Type& type, std::vector<Component*>& components);

	/// Get all components of type T into the provided vector
	void getComponents(const Type& type, std::vector<const Component*>& components) const;

	/// Get all components of the scene object
	std::vector<Component*> getComponents();

	/// Get all components of the scene object
	std::vector<const Component*> getComponents() const;

	/// Get all components of the scene objectinto the provided vector
	void getComponents(std::vector<Component*>& components);

	/// Get all components of the scene object into the provided vector
	void  getComponents(std::vector<const Component*>& component) const;

private:

	std::string mName;

	Transform mTransform;

	Scene* mScene{ nullptr };

	SceneObject* mParent{ nullptr };

	std::vector<SceneObject*> mChildren;

	std::vector<std::unique_ptr<Component>> mComponents;
};


template <ComponentType T>
T*
SceneObject::addComponent(bool autoActivate)
{
	return static_cast<T*>(addComponent(T::classType(), autoActivate));
}


template <ComponentType T>
T*
SceneObject::getComponent()
{
	return static_cast<T*>(findComponent(T::classType()));
}


template <ComponentType T>
const T*
SceneObject::getComponent() const
{
	return static_cast<T*>(findComponent(T::classType()));
}


template <ComponentType T>
std::vector<T*>
SceneObject::getComponents()
{
	return getComponents(T::classType())
		| std::views::transform([](auto c) { return static_cast<T*>(c); })
		| std::ranges::to<std::vector>();
}


template <ComponentType T>
std::vector<const T*>
SceneObject::getComponents() const
{
	return getComponents(T::classType())
		| std::views::transform([](auto c) { return static_cast<const T*>(c); })
		| std::ranges::to<std::vector>();
}

} // namespace Reef

#endif // !ENGINE_CORE_SCENEOBJECT_HPP