#include <Engine/Application/ApplicationImpl.hpp>

#include <Engine/Core/Scene.hpp>

#include <Engine/Core/SceneManager.hpp>
#include <Engine/Rendering/DefaultRenderer.hpp>
//#include <Rendering/Shaders/DefaultPBR.h>

#include <ranges>
#include <vector>
#include <memory>

//module Engine.Application.ApplicationImpl;
//
//
//import Engine.TypeRegistry;
//import Engine.SceneManager;
//
//import Engine.Shader;
//
//import Engine.Rendering.DefaultRenderer;
//
//import Common.SmartPointer;
//import Engine.ObjectFactory;

using namespace Reef;


ApplicationImpl::ApplicationImpl()
	: mSceneManager(std::make_shared<SceneManager>(*this)
	)
{

}


ApplicationImpl::~ApplicationImpl()
{

}


bool
ApplicationImpl::createDefaultShaders()
{
	/*auto defaultPBR = std::make_shared<Reef::Shader>();
	defaultPBR->setName(Reef::BuiltInShaders::DefaultPBR);
	defaultPBR->setVertexShaderSource(std::as_bytes(std::span(Shaders::DefaultPBR::vertexShaderSourceSpirV)));
	defaultPBR->setFragmentShaderSource(std::as_bytes(std::span(Shaders::DefaultPBR::fragmentShaderSourceSpirV)));
	
	if (!mAssetManager.addShader(Reef::BuiltInShaders::DefaultPBR, defaultPBR))
	{
		return false;
	}*/
	return true;
}


std::unique_ptr<ApplicationImpl>
ApplicationImpl::create(const CommandLineArguments& arguments)
{
	std::unique_ptr<ApplicationImpl> app(new ApplicationImpl);

	app->mWindow = Window::create("", 1024, 768, false, false);

	auto renderer = std::make_unique<Reef::Rendering::DefaultRenderer>();
	
	if (!renderer->initialize(*app))
	{
		return nullptr;
	}

	app->mRenderer = std::move(renderer);

	//if (!app->createDefaultShaders())
	//{
	//	return nullptr;
	//}
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
	return mSceneManager->activeScene();
}


SceneManager&
ApplicationImpl::sceneManager()
{
	return *mSceneManager;
}
//
//
//AssetManager&
//ApplicationImpl::assetManager()
//{
//	return mAssetManager;
//}


//ResourceManager&
//ApplicationImpl::resourceManager()
//{
//	return mResourceManager;
//}

Reef::Rendering::IRenderer&
ApplicationImpl::renderer()
{
	return *mRenderer;
}


void
ApplicationImpl::gameLoop()
{
	while (true)
	{
		mWindow->processEvents();

		if (mWindow->shouldClose())
		{
			quit(ReturnCode::SUCCESS);
		}

		if (isQuitting())
		{
			break;
		}

		auto scene = getActiveScene();


		if (!scene)
		{
			continue;
		}

		scene->update();

		mRenderer->render(scene->createSceneView());
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