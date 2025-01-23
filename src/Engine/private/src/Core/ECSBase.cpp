#include <Engine/Core/ECSBase.hpp>

using namespace Reef;


void
ECSBase::initialize(Application& application)
{
	mApplication = &application;
}


const Application&
ECSBase::application() const
{
	return *mApplication;
}


Application& 
ECSBase::application()
{
	return *mApplication;
}



