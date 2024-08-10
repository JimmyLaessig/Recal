module;

#include <iostream>

module Engine.Application.WindowLayer;

import Engine.Application.WindowImpl;

using namespace Reef;

bool
WindowLayer::initialize()
{
	mWindow = WindowImpl::create("Title", 2048, 1024, false, true);

	if (!mWindow)
	{
		return false;
	}

	mWidth = mWindow->width();
	mHeight = mWindow->height();

	return true;
}


Window*
WindowLayer::window()
{
	return mWindow.get();
}


uint32_t
WindowLayer::width()
{
	return mWidth;
}


uint32_t
WindowLayer::height()
{
	return mHeight;
}
