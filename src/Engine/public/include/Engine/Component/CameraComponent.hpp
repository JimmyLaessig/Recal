#ifndef ENGINE_COMPONENT_CAMERACOMPONENT_HPP
#define ENGINE_COMPONENT_CAMERACOMPONENT_HPP

#include <glm/glm.hpp>

#include <Engine/Core/Component.hpp>

#include <Engine/Core/Camera.hpp>

namespace Reef
{

class CameraComponent : public SubclassOf<CameraComponent, Component>
{
public:

	Reef::Camera camera() const;

	/// The size of the camera in pixel
	const glm::uvec2& size() const;

	/// Set the size of the camera in pixel
	void setSize(const glm::uvec2& size);

	/// Get the aspect ratio of the camera ( size.x / size.y)
	float aspect() const;

	/// Get the near plane of the camera
	float nearPlane() const;

	/// Set the near plane of the camera in scene units
	void setNearPlane(float near);

	/// Get the far plane of the camera
	float farPlane() const;

	/// Set the near plane of the camera in scene units
	void setFarPlane(float far);

	/// Set the vertical field of view in degrees
	void setVerticalFieldOfView(float fov);

	/// The vertical field of view in degrees
	float verticalFieldOfView() const;

	/// The projection matrix of the camera
	const glm::mat4& projectionMatrix() const;

	/// The view matrix of the camera transforms from world space to view space
	glm::mat4 viewMatrix() const;

private:

	void updateProjectionMatrix() const;

	mutable Camera mCamera{};

	mutable glm::mat4 mProjectionMatrix{};

	mutable bool mIsOutdated{ true };
};

} // namespace Reef


#endif // !ENGINE_COMPONENT_CAMERACOMPONENT_HPP