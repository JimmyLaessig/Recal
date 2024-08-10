module;

#include <memory>

export module Engine.Component.SkyLightComponent;

import Engine.Component.LightComponent;
import Engine.Texture;
import Engine.InheritsFrom;
import Engine.SkyLight;

export namespace Reef
{

class SkyLightComponent : public Engine::InheritsFrom<SkyLightComponent, LightComponent>
{
public:

	SkyLightComponent();

	std::shared_ptr<Engine::Texture> environmentTexture();

	void setEnvironmentTexture(std::shared_ptr<Engine::Texture> texture);

	std::shared_ptr<Engine::Texture> irradianceTexture();

	std::shared_ptr<Engine::Light> light() override { return mSkyLight; }

	void recalculateIrradianceTexture();

	private:

	std::shared_ptr<Engine::SkyLight> mSkyLight;
};

} // namespace Reef