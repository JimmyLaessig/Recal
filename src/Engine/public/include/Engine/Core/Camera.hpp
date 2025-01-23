#ifndef ENGINE_CORE_CAMERA_HPP
#define ENGINE_CORE_CAMERA_HPP

#include <Engine/System.hpp>

#include <glm/glm.hpp>

namespace Reef
{

struct ENGINE_API Camera
{
	glm::vec3 clearColor{ 0.f, 0.f, 0.f };

	glm::uvec2 size{ 1920u, 1080u };

	float nearPlane{ 0.01f };

	float farPlane{ 1000.f };

	float verticalFieldOfView{ 65.f };

	glm::mat4 projectionMatrix{};

	glm::mat4 inverseProjectionMatrix{};
};

} // namespace Reef

#endif // !ENGINE_CORE_CAMERA_HPP