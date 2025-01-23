#include <Engine/Core/Component.hpp>

#include <Engine/Core/SceneObject.hpp>
#include <Engine/Core/Scene.hpp>
#include <Engine/Core/GLTFLoader.hpp>
#include <Engine/Application/Application.hpp>

#include <string>
#include <string_view>
#include <filesystem>

using namespace Reef;


SceneObject& 
Component::owner()
{
	return *mOwner;
}


const SceneObject& 
Component::owner() const
{
	return *mOwner;
}


void
Component::setOwner(SceneObject& owner)
{
	mOwner = &owner;
}


//Transform&
//Component::transform()
//{
//	return mOwner->transform();
//}
//
//
//const Transform&
//Component::transform() const
//{
//	return mOwner->transform();
//}


void 
Component::setActive(bool active)
{
	if (mActive != active)
	{
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


SceneObject*
Component::createSceneObject(std::string_view name)
{
	return mOwner->scene().createSceneObject("name");
}


//const Time& 
//Component::time() const
//{
//	return mOwner->time();
//}


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
