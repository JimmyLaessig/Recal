module Engine.Application.Layer;

using namespace Reef;

bool
Layer::initialize(Application& application)
{
	mApplication = &application;

	return initialize();
}


bool 
Layer::initialize()
{ 
	return true;
}


void 
Layer::deinitialize()
{
}


void
Layer::start()
{
}


void
Layer::stop()
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