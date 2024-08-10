export module Engine.Component.DirectionalLightComponent;

import Engine.Component.LightComponent;
import Engine.InheritsFrom;
import Engine.DirectionalLight;

export namespace Reef
{

class DirectionalLightComponent : public InheritsFrom<DirectionalLightComponent, Engine::LightComponent>
{
public:
	
	DirectionalLightComponent();

	std::shared_ptr<Light> light() override;

private:

	std::shared_ptr<DirectionalLight> mDirectionalLight;
};

} // export namespace Reef