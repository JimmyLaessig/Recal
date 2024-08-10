module;

#include <vector>
#include <span>
#include <string_view>

export module Coral.UniformBlock;

import Coral.ShaderModule;

export namespace Coral
{


class UniformBlock
{
public:

	UniformBlock(const UniformBlockDescription& description);

	bool setValue(size_t index, bool value);

	bool setValue(size_t index, int value);

	bool setValue(size_t index, float value);

	bool setValue(std::string_view name, bool value);

	bool setValue(std::string_view name, int value);

	bool setValue(std::string_view name, float value);

	bool setValue(std::string_view name, ValueType type, const std::byte* value);

	bool setValue(size_t index, ValueType type, const std::byte* value);

	template<typename Vec2F>
	bool setVec2F(size_t index, const Vec2F& value);

	template<typename Vec3F>
	bool setVec3F(size_t index, const Vec3F& value);

	template<typename Vec4F>
	bool setVec4F(size_t index, const Vec4F& value);

	template<typename Mat33F>
	bool setMat33F(size_t index, const Mat33F& value);

	template<typename Mat44F>
	bool setMat44F(size_t index, const Mat44F& value);

	template<typename Vec2F>
	bool setVec2F(std::string_view name, const Vec2F& value);

	template<typename Vec3F>
	bool setVec3F(std::string_view name, const Vec3F& value);

	template<typename Vec4F>
	bool setVec4F(std::string_view name, const Vec4F& value);

	template<typename Mat33F>
	bool setMat33F(std::string_view name, const Mat33F& value);

	template<typename Mat44F>
	bool setMat44F(std::string_view name, const Mat44F& value);

	const std::span<const std::byte> data() const;

	size_t size() const;

private:

	UniformBlockDescription mDescription;

	std::vector<size_t> mByteOffsets;

	std::vector<std::byte> mData;
};

} // Coral


template<typename Vec2F>
bool
Coral::UniformBlock::setVec2F(size_t index, const Vec2F& value)
{
	return setValue(index, ValueType::VEC2F, reinterpret_cast<const std::byte*>(&value));
}


template<typename Vec3F>
bool 
Coral::UniformBlock::setVec3F(size_t index, const Vec3F& value)
{
	return setValue(index, ValueType::VEC3F, reinterpret_cast<const std::byte*>(&value));
}


template<typename Vec4F>
bool 
Coral::UniformBlock::setVec4F(size_t index, const Vec4F& value)
{
	return setValue(index, ValueType::VEC4F, reinterpret_cast<const std::byte*>(&value));
}


template<typename Mat33F>
bool
Coral::UniformBlock::setMat33F(size_t index, const Mat33F& value)
{
	return setValue(index, ValueType::MAT33F, reinterpret_cast<const std::byte*>(&value));
}


template<typename Mat44F>
bool
Coral::UniformBlock::setMat44F(size_t index, const Mat44F& value)
{
	return setValue(index, ValueType::MAT44F, reinterpret_cast<const std::byte*>(&value));
}


template<typename Vec2F>
bool 
Coral::UniformBlock::setVec2F(std::string_view name, const Vec2F& value)
{
	return setValue(name, ValueType::VEC2F, reinterpret_cast<const std::byte*>(&value));
}


template<typename Vec3F>
bool 
Coral::UniformBlock::setVec3F(std::string_view name, const Vec3F& value)
{
	return setValue(name, ValueType::VEC3F, reinterpret_cast<const std::byte*>(&value));
}


template<typename Vec4F>
bool 
Coral::UniformBlock::setVec4F(std::string_view name, const Vec4F& value)
{
	return setValue(name, ValueType::VEC4F, reinterpret_cast<const std::byte*>(&value));
}


template<typename Mat33F>
bool 
Coral::UniformBlock::setMat33F(std::string_view name, const Mat33F& value)
{
	return setValue(name, ValueType::MAT33F, reinterpret_cast<const std::byte*>(&value));
}


template<typename Mat44F>
bool 
Coral::UniformBlock::setMat44F(std::string_view name, const Mat44F& value)
{
	return setValue(name, ValueType::MAT44F, reinterpret_cast<const std::byte*>(&value));
}
