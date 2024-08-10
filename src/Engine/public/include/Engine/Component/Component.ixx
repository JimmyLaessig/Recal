module;

#include <Engine/System.h>

#include <string>
#include <string_view>

export module Engine.Component;

import Engine.Object;
import Engine.SubclassOf;
import Engine.Time;
import Engine.Transform;

export namespace Reef
{

class GameObject;

class ENGINE_API Component : public SubclassOf<Component, Object>
{
public:

	Component();

	GameObject& owner();

	const GameObject& owner() const;

	Transform& transform(); 

	const Transform& transform() const;

	void setActive(bool active);

	bool isActive() const;

	const std::string& name() const;

	void setName(std::string_view name);

	bool isBeingDestroyed() const;

protected:

	GameObject* createGameObject(std::string_view name = "GameObject");

	const Time& time() const;

	//const Application& application() const;

	//Application& application();

	virtual void start();

	virtual void stop();

	virtual void onComponentActivated();

	virtual void onComponentDeactivated();

	virtual void onComponentCreated();

	virtual void onComponentDestroyed();

private:

	friend class GameObject;
	friend class Scene;

	std::string mName;

	bool mActive{ false };

	GameObject* mOwner{ nullptr };

	bool mIsBeingDestroyed{ false };
};

} // export namespace Reef