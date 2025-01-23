#ifndef ENGINE_CORE_RANGE1D_HPP
#define ENGINE_CORE_RANGE1D_HPP

#include <Engine/System.hpp>

#include <cassert>
#include <span>
#include <optional>


namespace Reef
{

class ENGINE_API Range1D
{
public:
	Range1D(float min, float max);

	Range1D(std::span<float> values);

	float min() const;

	float max() const;

	float center() const;
	
	float size() const;

	Range1D& operator+=(const Range1D& rhs);

	Range1D operator+(const Range1D& rhs) const;

	Range1D& operator+=(float value);

	Range1D operator+(float value) const;

	bool contains(float value) const;

	static std::optional<Range1D> intersect(const Range1D& lhs, const Range1D& rhs);

	template<typename ...Ranges>
	static Range1D combine(const Ranges&... ranges);

private:

	float mMin{ 0.f };
	float mMax{ 0.f };
};


template<typename ...Ranges>
Range1D
Range1D::combine(const Ranges&... ranges)
{
	assert(false);
	return {};
}

} // namespace Reef

#endif // !ENGINE_CORE_RANGE1D_HPP