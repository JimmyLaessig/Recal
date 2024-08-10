module;

#include <glm/glm.hpp>

export module Engine.Camera;

export namespace Reef
{

struct Camera
{
	glm::uvec2 size{ 1920u, 1080u };

	float nearPlane{ 0.01f };

	float farPlane{ 1000.f };

	float verticalFieldOfView{ 65.f };

	glm::mat4 projectionMatrix{};

	glm::mat4 inverseProjectionMatrix{};
};

} // namespace Reef