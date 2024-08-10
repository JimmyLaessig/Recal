module;

#include <glm/glm.hpp>

#include <memory>

export module Engine.DirectionalLight;

import Engine.Light;

export namespace Reef
{

class DirectionalLight : public Light
{

public:

	Light::Type getType() const override;

	const glm::vec3& color() const override;

	void setColor(const glm::vec3& color) override;

	bool setCastShadows(bool enabled) override;

	bool castShadows() const override;

	std::shared_ptr<Engine::Rendering::RenderResource> RenderResource() override;

	const glm::vec3& direction() const;

	void setDirection(const glm::vec3& direction);

	const glm::mat4& lightSpaceMatrix() const;

private:

	bool mCastShadows{ false };

	glm::vec3 mColor{ glm::vec3(1.f, 1.f, 1.f) };

	glm::vec3 mLightDirection{ glm::normalize(glm::vec3(1.f, 1.f, 1.f)) };

	glm::mat4 mLightSpaceMatrix;

};

} // namespace Reef