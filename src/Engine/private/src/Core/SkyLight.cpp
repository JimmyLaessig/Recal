module;

#include <glm/glm.hpp>

#include <memory>

module Engine.SkyLight;

import Engine.Rendering.Renderer;
import Engine.Rendering.CalculateIrradiance;

using namespace Reef;

Light::Type
SkyLight::getType() const
{
	return Light::Type::Sky;
}


const glm::vec3&
SkyLight::color() const
{
	return mColor;
}


void
SkyLight::setColor(const glm::vec3& color)
{
	mColor = color;
}


bool
SkyLight::setCastShadows(bool enabled)
{
	return false;
}


bool
SkyLight::castShadows() const
{
	return false;
}


std::shared_ptr<Engine::Texture>
SkyLight::skyLightTexture()
{
	return mSkyLightTexture;
}


std::shared_ptr<Engine::Texture>
SkyLight::irradianceTexture()
{
	return mIrradianceTexture;
}


void
SkyLight::setSkyLightTexture(std::shared_ptr<Engine::Texture> texture)
{
	mSkyLightTexture = texture;
	recalculateIrradianceTexture();
}


void
SkyLight::recalculateIrradianceTexture()
{
	mIrradianceTexture.reset();
	mIrradianceTexture = std::make_shared<Engine::Texture>();
	mIrradianceTexture->set(512, 256, Engine::TextureFormat::RGB_16F);

	Engine::Rendering::CalculateIrradiance task;

	auto& renderer = Rendering::Renderer::instance();
	if (task.initialize(renderer.context(), mSkyLightTexture, mIrradianceTexture))
	{
		renderer.submitCommandBuffer(task.recordRenderCommands());
	}
}
