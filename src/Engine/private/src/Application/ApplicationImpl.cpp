module;

#include <ranges>
#include <vector>
#include <memory>

module Engine.Application.ApplicationImpl;

import Engine.Application.WindowLayer;

import Engine.TypeRegistry;
import Engine.SceneManager;

import Common.SmartPointer;
//import Engine.ObjectFactory;

using namespace Reef;


ApplicationImpl::ApplicationImpl()
	: mSceneManager(*this)
	, mShaderManager(*this)
{

}


std::unique_ptr<ApplicationImpl>
ApplicationImpl::create(const CommandLineArguments& arguments)
{
	std::unique_ptr<ApplicationImpl> app(new ApplicationImpl);

	app->mWindow = Window::create("", 1024, 768, false, false);
	//auto layer = app->addLayer<Reef::WindowLayer>();



	Coral::RAII::ContextPtr mContext;

	if (!app->mRenderer.initialize(*app))
	{
		return nullptr;
	}

	return app;
}


void
ApplicationImpl::quit(ReturnCode returnCode)
{
	mReturnCode = returnCode;
}


bool
ApplicationImpl::isQuitting() const
{
	return mReturnCode.has_value();
}


Window*
ApplicationImpl::window()
{
	return mWindow.get();
}


ReturnCode
ApplicationImpl::run()
{
	if (mIsRunning)
	{
		return ReturnCode::FAILURE;
	}

	mIsRunning = true;

	gameLoop();

	return mReturnCode.value_or(ReturnCode::FAILURE);
}


Scene*
ApplicationImpl::getActiveScene()
{
	return mSceneManager.activeScene();
}


SceneManager&
ApplicationImpl::sceneManager()
{
	return mSceneManager;
}


ResourceManager&
ApplicationImpl::resourceManager()
{
	return mResourceManager;
}


IShaderManager&
ApplicationImpl::shaderManager()
{
	return mShaderManager;
}


IRenderer&
ApplicationImpl::renderer()
{
	return mRenderer;
}


Coral::Context*
ApplicationImpl::renderContext()
{
	return mRenderContext.get();
}


void
ApplicationImpl::gameLoop()
{
	while (true)
	{
		mWindow->processEvents();

		if (auto scene = getActiveScene())
		{
			scene->update();
		}

		if (mWindow->shouldClose())
		{
			quit(ReturnCode::SUCCESS);
		}

		if (isQuitting())
		{
			break;
		}
	}
}


//Layer* 
//ApplicationImpl::addLayer2(const Type& type)
//{
//	if (!type.isSubclassOf(Layer::classType()))
//	{
//		return nullptr;
//	}
//	
//	auto ctor = TypeRegistry::constructor(type);
//
//	if (!ctor)
//	{
//		return nullptr;
//	}
//
//	auto layer = Common::staticPointerCast<Layer>(ctor());
//
//	if (!layer)
//	{
//		return nullptr;
//	}
//
//	if (!layer->initialize(*this))
//	{
//		return nullptr;
//	}
//
//	auto ptr = mLayers.emplace_back(std::move(layer)).get();
//
//	return ptr;
//}


//Layer*
//ApplicationImpl::findLayer2(const Type& type)
//{
//	auto iter = std::find_if(mLayers.begin(), mLayers.end(), [&](const auto& layer) { return layer->type() == type; });
//
//	if (iter == mLayers.end())
//	{
//		return nullptr;
//	}
//
//	return iter->get();
//}
//
//
//const Layer*
//ApplicationImpl::findLayer2(const Type& type) const
//{
//	auto iter = std::find_if(mLayers.begin(), mLayers.end(), [&](const auto& layer) { return layer->type() == type; });
//
//	if (iter == mLayers.end())
//	{
//		return nullptr;
//	}
//
//	return iter->get();
//}