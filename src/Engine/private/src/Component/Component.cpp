module;


#include <string>
#include <string_view>

module Engine.Component;

import Engine.GameObject;
import Engine.Scene;
import Engine.Transform;

using namespace Reef;

Component::Component()
{
	mName = type().name();
}


GameObject& 
Component::owner()
{
	return *mOwner;
}


const GameObject& 
Component::owner() const
{
	return *mOwner;
}


Transform&
Component::transform()
{
	return mOwner->transform();
}


const Transform&
Component::transform() const
{
	return mOwner->transform();
}


void 
Component::setActive(bool active)
{
	if (mActive == active)
	{
		return;
	}

	mActive = active;

	if (mActive)
	{
		onComponentActivated();
	}
	else
	{
		onComponentDeactivated();
	}
}


bool 
Component::isActive() const
{
	return mActive;
}


void
Component::start()
{

}


void
Component::stop()
{

}


void
Component::onComponentActivated()
{

}


void
Component::onComponentDeactivated()
{

}


void
Component::onComponentCreated()
{
}


void
Component::onComponentDestroyed()
{
}


const
std::string&
Component::name() const
{
	return mName;
}


void
Component::setName(std::string_view name)
{
	mName = name;
}


GameObject*
Component::createGameObject(std::string_view name)
{
	return nullptr;
}


const Time& 
Component::time() const
{
	return mOwner->time();
}


//const Application&
//Component::application() const
//{
//	return mOwner->application();
//}
//
//
//Application&
//Component::application()
//{
//	return mOwner->application();
//}


bool
Component::isBeingDestroyed() const
{
	return mIsBeingDestroyed;
}
