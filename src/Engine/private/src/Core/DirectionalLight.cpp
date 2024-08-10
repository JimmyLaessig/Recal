module;

#include <glm/glm.hpp>

module Engine.DirectionalLight;

using namespace Reef;

Light::Type
DirectionalLight::getType() const
{
	return Light::Type::Directional;
}


const glm::vec3&
DirectionalLight::color() const
{
	return mColor;
}


void
DirectionalLight::setColor(const glm::vec3& color)
{
	mColor = color;
}


bool
DirectionalLight::setCastShadows(bool enabled)
{
	mCastShadows = enabled;
	return true;
}


bool
DirectionalLight::castShadows() const
{
	return mCastShadows;
}


const glm::vec3&
DirectionalLight::direction() const
{
	return mLightDirection;
}


void
DirectionalLight::setDirection(const glm::vec3& direction)
{
	mLightDirection = glm::normalize(direction);
}


std::shared_ptr<Engine::Rendering::RenderResource>
DirectionalLight::RenderResource()
{
	return nullptr;
}


const glm::mat4&
DirectionalLight::lightSpaceMatrix() const
{
	return mLightSpaceMatrix;
}