module;

#include <cstdint>
#include <memory>
#include <string>
#include <string_view>
#include <variant>
#include <optional>
#include <vector>

export module Engine.Application.ApplicationImpl;

//import Engine.Application.Input;
import Engine.Application;
import Engine.Application.Layer;
import Engine.Application.CommandLineArguments;
import Engine.Application.Window;

import Engine.ResourceManager;
import Engine.SceneManager;
import Engine.ShaderManagerImpl;

import Engine.Rendering.DefaultRenderer;

import Coral.RAII;

export namespace Reef
{

class ApplicationImpl : public IApplication
{
public:

	static std::unique_ptr<ApplicationImpl> create(const CommandLineArguments& arguments);

	void quit(ReturnCode exitCode) override;

	Window* window() override;

	ReturnCode run() override;

	bool isQuitting() const override;

	Scene* getActiveScene() override;

	SceneManager& sceneManager() override;

	ResourceManager& resourceManager() override;

	IShaderManager& shaderManager() override;

	IRenderer& renderer() override;

	Coral::Context* renderContext();

private:

	void gameLoop();

	ApplicationImpl();

	CommandLineArguments mCommandLineArguments;

	std::unique_ptr<Window> mWindow;

	bool mIsRunning{ false };

	std::optional<ReturnCode> mReturnCode;

	SceneManager mSceneManager;

	ResourceManager mResourceManager;

	ShaderManagerImpl mShaderManager;

	DefaultRenderer mRenderer;

	Coral::RAII::ContextPtr mRenderContext;
};

} // export namespace Reef