module;

#include <Engine/System.h>

#include <memory>

export module Engine.Application;

import Engine.Application.CommandLineArguments;
import Engine.Application.Window;

import Engine.ResourceManager;
import Engine.Scene;
import Engine.SceneManager;
import Engine.ShaderManager;
import Engine.Type;
import Engine.Renderer;

export namespace Reef
{

enum class ReturnCode
{
	SUCCESS = 0,
	FAILURE = 1,
};


class ENGINE_API IApplication
{
public:

	virtual ~IApplication() = default;

	virtual ReturnCode run() = 0;

	virtual Window* window() = 0;

	virtual void quit(ReturnCode returnCode = ReturnCode::SUCCESS) = 0;

	virtual bool isQuitting() const = 0;

	virtual Scene* getActiveScene() = 0;

	virtual SceneManager& sceneManager() = 0;

	virtual ResourceManager& resourceManager() = 0;

	virtual IShaderManager& shaderManager() = 0;

	virtual IRenderer& renderer() = 0;
};

ENGINE_API std::unique_ptr<IApplication> createApplication(const CommandLineArguments& arguments);

} // export namespace Reef


//template<typename T>
//T* Reef::Application::addLayer()
//{
//	return static_cast<T*>(this->addLayer2(T::classType()));
//}
//
//
//template<typename T>
//T* Reef::Application::findLayer()
//{
//	return static_cast<T*>(this->findLayer2(T::classType()));
//}
