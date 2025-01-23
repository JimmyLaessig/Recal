#ifndef ENGINE_ECS_COMPONENT_HPP
#define ENGINE_ECS_COMPONENT_HPP

#include <Engine/System.hpp>

#include <Engine/Core/ECSBase.hpp>
#include <Engine/Core/Transform.hpp>

#include <string>
#include <string_view>

#include <nlohmann/json.hpp>

namespace Reef
{

class Scene;
class SceneObject;


class ENGINE_API Component : public SubclassOf<Component, ECSBase>
{
public:

	SceneObject& owner();

	const SceneObject& owner() const;

	void setOwner(SceneObject& owner);

	void setActive(bool active);

	bool isActive() const;

	const std::string& name() const;

	void setName(std::string_view name);

	Transform& transform();

	const Transform& transform() const;

	bool isBeingDestroyed() const;

	virtual void start();

	virtual void stop();

	virtual void onComponentActivated();

	virtual void onComponentDeactivated();

	virtual void onComponentCreated();

	virtual void onComponentDestroyed();

protected:

	SceneObject* createSceneObject(std::string_view name = "SceneObject");

	//const Time& time() const;

private:

	std::string mName;

	bool mActive{ false };

	SceneObject* mOwner{ nullptr };

	bool mIsBeingDestroyed{ false };
};

} // namespace Reef

#endif // !ENGINE_ECS_COMPONENT_HPP