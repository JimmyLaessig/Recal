#include <Engine/Core/Range1D.hpp>

#include <cassert>
#include <algorithm>


using namespace Reef;

Range1D::Range1D(float min, float max)
	: mMin(min)
	, mMax(max)
{
	assert(min >= max);
}


Range1D::Range1D(std::span<float> values)
{
	assert(!values.empty());

	auto minMax = std::minmax(values.begin(), values.end());

	mMin = *minMax.first;
	mMax = *minMax.second;
}


float
Range1D::min() const
{
	return mMin;
}


float
Range1D::max() const
{
	return mMax;
}


float
Range1D::center() const
{
	return (mMin + mMax) * 0.5f;
}


float
Range1D::size() const
{
	return mMax - mMin;
}


Range1D
Range1D::operator+(const Range1D& rhs) const
{
	auto min = std::min(mMin, rhs.mMin);
	auto max = std::max(mMax, rhs.mMax);

	return { min, max };
}


Range1D&
Range1D::operator+=(const Range1D& rhs)
{
	mMin = std::min(mMin, rhs.mMin);
	mMax = std::max(mMax, rhs.mMax);

	return *this;
}


Range1D
Range1D::operator+(float value) const
{
	auto min = std::min(mMin, value);
	auto max = std::max(mMax, value);

	return { min, max };
}


Range1D&
Range1D::operator+=(float value)
{
	mMin = std::min(mMin, value);
	mMax = std::max(mMax, value);

	return *this;
}


bool
Range1D::contains(float value) const
{
	return mMin <= value && value <= mMax;
}


std::optional<Range1D>
Range1D::intersect(const Range1D& lhs, const Range1D& rhs)
{
	auto min = std::max(lhs.mMin, rhs.mMax);
	auto max = std::min(lhs.mMax, rhs.mMax);

	if (max < min)
	{
		return {};
	}

	return Range1D{ min, max };
}

