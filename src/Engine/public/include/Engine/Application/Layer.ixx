module;

#include <Engine/System.h>

export module Engine.Application.Layer;

import Engine.Application.CommandLineArguments;

import Engine.Object;
import Engine.SubclassOf;

export namespace Reef
{
class Application;

class ENGINE_API Layer : public SubclassOf<Layer, Object>
{
public:

	virtual bool initialize(Application& application);

	virtual void start();

	virtual void update();

	Application& application();

	const Application& application() const;

private:

	Application* mApplication{ nullptr };
};

} // export namespace Reef