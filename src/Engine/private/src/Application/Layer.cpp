module;


module Engine.Application.Layer;

import Engine.Application;

using namespace Reef;


bool
Layer::initialize(Application& application)
{
	mApplication = &application;

	return true;
};


void
Layer::start()
{

}


void
Layer::update()
{

}


Application&
Layer::application()
{
	return *mApplication;
}

const Application&
Layer::application() const
{
	return *mApplication;
}