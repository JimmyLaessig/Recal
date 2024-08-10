module;


module Engine.Application.WindowLayer;

import Engine.Application;
import Engine.Application.WindowImpl;

using namespace Reef;


bool
WindowLayer::initialize(Application& application)
{
	if (!Layer::initialize(application))
	{
		return false;
	}

	mWindow = Reef::Window::create("", 1024, 768, false, true);

	if (!mWindow)
	{
		return false;
	}

	return true;
}


Window*
WindowLayer::window()
{
	return mWindow.get();
}


void
WindowLayer::update()
{
	mWindow->processEvents();
}