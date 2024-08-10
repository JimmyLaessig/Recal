module;

#include <memory>

module Engine.Component.DirectionalLightComponent;

import Engine.DirectionalLight;


using namespace Reef;


DirectionalLightComponent::DirectionalLightComponent()
	: mDirectionalLight(std::make_shared<DirectionalLight>())
{
}

std::shared_ptr<Light> DirectionalLightComponent::light()
{
	return mDirectionalLight;
}
