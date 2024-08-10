module;

#include <glm/glm.hpp>

export module Engine.BoundingBox3D;

import Engine.Range1D;

export namespace Reef
{

class BoundingBox3D
{
public:

	BoundingBox3D() = default;

	BoundingBox3D(const glm::vec3& min, const glm::vec3& max);

	glm::vec3 min() const;

	glm::vec3 max() const;

	glm::vec3 center() const;

	glm::vec3 size() const;

	void add(const glm::vec3& p);

	void add(const BoundingBox3D& box);

	BoundingBox3D& operator+=(const BoundingBox3D& rhs);

	BoundingBox3D& operator+=(const glm::vec3& rhs);

private:

	glm::vec3 mMin;

	glm::vec3 mMax;

	bool mValid{ false };
};

BoundingBox3D operator+(const BoundingBox3D& lhs, const BoundingBox3D& rhs);

BoundingBox3D operator+(const BoundingBox3D& lhs, const glm::vec3& rhs);

BoundingBox3D operator+(const glm::vec3& lhs, const BoundingBox3D& rhs);

} // namespace Reef