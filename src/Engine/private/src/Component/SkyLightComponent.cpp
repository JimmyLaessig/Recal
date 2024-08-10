module;

#include <memory>

module Engine.Component.SkyLightComponent;

using namespace Reef;

SkyLightComponent::SkyLightComponent()
	: mSkyLight(std::make_shared<SkyLight>())
{

}

std::shared_ptr<Engine::Texture>
SkyLightComponent::environmentTexture()
{
	return mSkyLight->skyLightTexture();
}


std::shared_ptr<Engine::Texture>
SkyLightComponent::irradianceTexture()
{
	return mSkyLight->irradianceTexture();;
}


void
SkyLightComponent::setEnvironmentTexture(std::shared_ptr<Engine::Texture> texture)
{
	mSkyLight->setSkyLightTexture(texture);
}


void 
SkyLightComponent::recalculateIrradianceTexture()
{

}