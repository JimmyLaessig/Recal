#include <Engine/Application/Layer.hpp>
#include <Engine/Application/Application.hpp>


using namespace Reef;


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