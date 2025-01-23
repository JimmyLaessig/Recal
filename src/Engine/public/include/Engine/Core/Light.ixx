module;

#include <glm/glm.hpp>

#include <memory>
#include <vector>
#include <unordered_map>
#include <span>

export module Engine.Light;

import Engine.BoundingBox3D;
import Engine.Material;

namespace Reef
{

namespace Rendering
{
class RenderResource;
}

}

export namespace Reef
{

class Light
{

public:

	enum class Type
	{
		Sky,
		Directional,
		Point,
		Cone
	};

	virtual Light::Type getType() const = 0;

	virtual const glm::vec3& color() const = 0;

	virtual void setColor(const glm::vec3& color) = 0;

	virtual bool setCastShadows(bool enabled) = 0;

	virtual bool castShadows() const = 0;

	virtual std::shared_ptr<Engine::Rendering::RenderResource> RenderResource() = 0;
};

} // namespace Reef