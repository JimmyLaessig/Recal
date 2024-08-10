module;

#include <Engine/System.h>

#include <glm/glm.hpp>

#include <cstdint>
#include <vector>
#include <memory>

export module Engine.Mesh;

import Engine.Object;
import Engine.Primitive;
import Engine.SubclassOf;

import Engine.BoundingBox3D;
import Engine.Material;

export namespace Reef
{

class ENGINE_API Mesh : public SubclassOf<Mesh, Object>
{
public:

	std::shared_ptr<Primitive> getPrimitive(uint32_t index);

	std::shared_ptr<const Primitive> getPrimitive(uint32_t index) const;

	const std::vector<std::shared_ptr<Primitive>>& primitives();

	std::shared_ptr<Material> material(uint32_t index);

	uint32_t getNumPrimitives() const;

	Primitive& addPrimitive();

	BoundingBox3D boundingBox() const;

	static std::shared_ptr<Mesh> Sphere();

	static std::shared_ptr<Mesh> Sphere(uint32_t numSubdivisions);

	static std::shared_ptr<Mesh> Quad();

	static std::shared_ptr<Mesh> Cube();

	static std::shared_ptr<Mesh> Plane();

private:

	std::vector<std::shared_ptr<Primitive>> mPrimitives;

	BoundingBox3D mBoundingBox{ glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 0.f, 0.f) };
};

} // namespace Reef