module;

#include <Engine/System.h>

#include <chrono>

export module Engine.World;

import Event;

export namespace Reef
{

class ENGINE_API World
{
public:

	Common::Event<> OnFrameUpdateEvent;

	float deltaTime() const;

	void start();

	void updateFrame();

	void stop();

private:

	float mDeltaTime{ 0.f };

	std::chrono::time_point<std::chrono::system_clock> mLastUpdateTS{ std::chrono::system_clock::now() };

}; // class Engine

} // export namespace Reef