#ifndef REEF_APPLICATION_APPLICATION_HPP
#define REEF_APPLICATION_APPLICATION_HPP

#include <Engine/System.hpp>

#include <Engine/Application/CommandLineArguments.hpp>
#include <Engine/Application/Window.hpp>

#include <memory>

namespace Reef::Rendering
{
	class IRenderer;
} // namespace Reef::Rendering

namespace Reef
{


class Scene;
class AssetImporter;
class SceneManager;

enum class ReturnCode
{
	SUCCESS = 0,
	FAILURE = 1,
};


class ENGINE_API Application
{
public:

	virtual ~Application() = default;

	virtual ReturnCode run() = 0;

	virtual Window* window() = 0;

	virtual void quit(ReturnCode returnCode = ReturnCode::SUCCESS) = 0;

	virtual bool isQuitting() const = 0;

	virtual Scene* getActiveScene() = 0;

	virtual SceneManager& sceneManager() = 0;

	/*virtual AssetManager& assetManager() = 0;*/

	virtual Rendering::IRenderer& renderer() = 0;
};

ENGINE_API std::unique_ptr<Application> createApplication(const CommandLineArguments& arguments);

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

#endif // !REEF_APPLICATION_APPLICATION_HPP