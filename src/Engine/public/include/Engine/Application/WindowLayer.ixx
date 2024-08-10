module;

#include <Engine/System.h>

#include <memory>

export module Engine.Application.WindowLayer;

import Engine.Application.Layer;
import Engine.Application.Window;

import Engine.SubclassOf;

export namespace Reef
{

class ENGINE_API WindowLayer : public SubclassOf<WindowLayer, Layer>
{

public:

	bool initialize(Application& application) override;

	void update() override;

	Window* window();

private:

	std::unique_ptr<Window> mWindow;
};

} // namespace Reef
