module;

#include <Engine/System.hpp>

#include <cstdint>

export module Engine.Time;

export namespace Reef
{

using Timepoint = uint64_t;


struct ENGINE_API Time
{
	/// The timestamp of the current frame in nanoseconds
	Timepoint currentFrameTimestamp;

	/// The time since the last update in seconds
	float deltaTime;

	/// Get the current timestamp in nanoseconds
	static Timepoint now();

};

} // export namespace Reef
