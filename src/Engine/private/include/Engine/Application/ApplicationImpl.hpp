#ifndef REEF_APPLICATION_APPLICATIONIMPL_HPP
#define REEF_APPLICATION_APPLICATIONIMPL_HPP

#include <Engine/System.hpp>
#include <Engine/Application/Application.hpp>

#include <cstdint>
#include <memory>
#include <string>
#include <string_view>
#include <variant>
#include <optional>
#include <vector>
#include <unordered_map>

namespace Reef
{

class ApplicationImpl : public Application
{
public:

	~ApplicationImpl();

	static std::unique_ptr<ApplicationImpl> create(const CommandLineArguments& arguments);

	void quit(ReturnCode exitCode) override;

	Window* window() override;

	ReturnCode run() override;

	bool isQuitting() const override;

	Scene* getActiveScene() override;

	SceneManager& sceneManager() override;

	//AssetManager& assetManager() override;

	////ResourceManager& resourceManager() override;

	Reef::Rendering::IRenderer& renderer() override;

private:

	bool createDefaultShaders();

	void gameLoop();

	ApplicationImpl();

	CommandLineArguments mCommandLineArguments;

	std::unique_ptr<Window> mWindow;

	bool mIsRunning{ false };

	std::optional<ReturnCode> mReturnCode;

	std::unique_ptr<Reef::Rendering::IRenderer> mRenderer;

	std::shared_ptr<SceneManager> mSceneManager;

	//AssetManager mAssetManager;

};

} // namespace Reef

#endif // !REEF_APPLICATION_APPLICATIONIMPL_HPP