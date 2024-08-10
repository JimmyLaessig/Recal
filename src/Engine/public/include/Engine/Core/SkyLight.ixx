module;

#include <glm/glm.hpp>

#include <memory>

export module Engine.SkyLight;

import Engine.Light;
import Engine.Texture;

export namespace Reef
{

class SkyLight : public Light
{

public:

	Light::Type getType() const override;

	const glm::vec3& color() const override;

	void setColor(const glm::vec3& color) override;

	bool setCastShadows(bool enabled) override;

	bool castShadows() const override;

	std::shared_ptr<Engine::Rendering::RenderResource> RenderResource() override { return nullptr; }

	std::shared_ptr<Engine::Texture> skyLightTexture();

	std::shared_ptr<Engine::Texture> irradianceTexture();

	void setSkyLightTexture(std::shared_ptr<Engine::Texture> texture);

	void recalculateIrradianceTexture();

private:

	glm::vec3 mColor{ glm::vec3(1.f, 1.f, 1.f) };

	std::shared_ptr<Engine::Texture> mSkyLightTexture;

	std::shared_ptr<Engine::Texture> mIrradianceTexture;
};

} // namespace Reef