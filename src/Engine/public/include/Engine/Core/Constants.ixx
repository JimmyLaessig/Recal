module;

#include <Engine/System.h>

#include <concepts>

export module Engine.Constants;

export namespace Reef
{

template<std::floating_point FLOAT>
class Constants
{
public:

	Constants() = delete;

	constexpr static FLOAT PI{ static_cast<FLOAT>(3.14159265358979323846) };

	constexpr static FLOAT HALF_PI{ static_cast<FLOAT>(PI * 0.5) };

	constexpr static FLOAT NANOSECONDS_TO_SECONDS{ static_cast<FLOAT>(1e-9) };

	constexpr static FLOAT MICROSECONDS_TO_SECONDS{ static_cast<FLOAT>(1e-3) };

	constexpr static FLOAT MILLISECONDS_TO_SECONDS{ static_cast<FLOAT>(1e-3) };

	constexpr static FLOAT RADIANS_TO_DEGREES{ static_cast<FLOAT>(180 / PI) };

	constexpr static FLOAT DEGREES_TO_RADIANS{ static_cast<FLOAT>(PI / 180) };
};

} // namespace Reef