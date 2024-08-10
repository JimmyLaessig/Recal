module;

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

module Engine.Component.CameraComponent;

import Engine.GameObject;

using namespace Reef;

Camera 
CameraComponent::camera() const
{
	updateProjectionMatrix();
	return mCamera;
}


const glm::uvec2&
CameraComponent::size() const
{
	return mCamera.size;
}


void
CameraComponent::setSize(const glm::uvec2& size)
{
	mIsOutdated = true;
	mCamera.size = size;
}


float
CameraComponent::aspect() const
{
	return static_cast<float>(mCamera.size[0]) / mCamera.size[1];
}


float
CameraComponent::nearPlane() const
{
	return mCamera.nearPlane;
}


float
CameraComponent::farPlane() const
{
	return mCamera.farPlane;
}


void
CameraComponent::setNearPlane(float near)
{
	mCamera.nearPlane = near;
	mIsOutdated = true;
}


void
CameraComponent::setFarPlane(float far)
{
	mCamera.farPlane = far;
	mIsOutdated = true;
}


float
CameraComponent::verticalFieldOfView() const
{
	return mCamera.verticalFieldOfView;
}


void
CameraComponent::setVerticalFieldOfView(float fov)
{
	mCamera.verticalFieldOfView = fov;
	mIsOutdated = true;
}


const glm::mat4&
CameraComponent::projectionMatrix() const
{
	updateProjectionMatrix();
	return mCamera.projectionMatrix;
}


glm::mat4
CameraComponent::viewMatrix() const
{
	return owner().transform().worldToLocalMatrix();
}


void 
CameraComponent::updateProjectionMatrix() const
{
	if (mIsOutdated)
	{
		mIsOutdated = false;
		
		mCamera.projectionMatrix = glm::perspective(glm::radians(mCamera.verticalFieldOfView), aspect(), mCamera.nearPlane, mCamera.farPlane);
		mCamera.inverseProjectionMatrix = glm::inverse(mCamera.projectionMatrix);
	}
}