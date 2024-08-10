module;

#include <glm/glm.hpp>

module Engine.BoundingBox3D;

using namespace Reef;


BoundingBox3D::BoundingBox3D(const glm::vec3& min, 
							 const glm::vec3& max)
{
	add(min);
	add(max);
}


glm::vec3
BoundingBox3D::min() const
{
	return mMin;
}


glm::vec3
BoundingBox3D::max() const
{
	return mMax;
}


glm::vec3
BoundingBox3D::center() const
{
	return (mMin + mMax) * 0.5f;
}


glm::vec3
BoundingBox3D::size() const
{
	return mMax - mMin;
}


void 
BoundingBox3D::add(const glm::vec3& p)
{
	if (!mValid)
	{
		mMin = p;
		mMax = p;
	}
	else
	{
		mMin = glm::min(mMin, p);
		mMax = glm::max(mMax, p);
	}

	mValid = true;
}


void 
BoundingBox3D::add(const BoundingBox3D& box)
{
	if (!mValid)
	{
		*this = box;
	}
	else if (box.mValid)
	{
		mMin = glm::min(mMin, box.mMin);
		mMax = glm::max(mMax, box.mMax);
		mValid = true;
	}
}


BoundingBox3D&
BoundingBox3D::operator+=(const BoundingBox3D& rhs)
{
	add(rhs);
	return *this;
}


BoundingBox3D& 
BoundingBox3D::operator+=(const glm::vec3& value)
{
	add(value);
	return *this;
}

namespace Reef
{

BoundingBox3D
operator+(const BoundingBox3D& lhs, const BoundingBox3D& rhs)
{
	BoundingBox3D result = lhs;
	result += rhs;

	return result;
}


BoundingBox3D
operator+(const BoundingBox3D& lhs, const glm::vec3& rhs)
{
	BoundingBox3D result = lhs;
	result += rhs;

	return result;
}


BoundingBox3D
operator+(const glm::vec3& lhs, const BoundingBox3D& rhs)
{
	return rhs + lhs;
}

} // namespace Reef
