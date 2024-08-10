module;

#include <cassert>
#include <span>
#include <string>
#include <string_view>
#include <vector>

module Coral.UniformBlock;

using namespace Coral;


constexpr size_t
byteStride(ValueType type)
{
	switch (type)
	{
		case ValueType::BOOL:	return 4;
		case ValueType::INT:	return 4;
		case ValueType::FLOAT:	return 4;
		case ValueType::VEC2F:	return 8;
		case ValueType::VEC3F:	return 16;
		case ValueType::VEC4F:	return 16;
		case ValueType::MAT33F:	return 48;
		case ValueType::MAT44F:	return 64;
		default:
			assert(false);
			return 0;
	};
}


constexpr size_t
byteSize(ValueType type)
{
	switch (type)
	{
		case ValueType::BOOL:	return 4;
		case ValueType::INT:	return 4;
		case ValueType::FLOAT:	return 4;
		case ValueType::VEC2F:	return 8;
		case ValueType::VEC3F:	return 12;
		case ValueType::VEC4F:	return 16;
		case ValueType::MAT33F:	return 36;
		case ValueType::MAT44F:	return 64;
		default:
			assert(false);
			return 0;
	};
}


UniformBlock::UniformBlock(const UniformBlockDescription& description)
	: mDescription(description)
{
	size_t bufferSize{ 0 };
	for (size_t i = 0; i < mDescription.members.size(); ++i)
	{
		mByteOffsets.push_back(bufferSize);
		bufferSize += byteStride(mDescription.members[i].type);
	}

	mData.resize(bufferSize, std::byte(0));
}


bool
UniformBlock::setValue(size_t index, bool value)
{
	int v = value;
	return setValue(index, ValueType::BOOL, reinterpret_cast<const std::byte*>(&v));
}


bool
UniformBlock::setValue(size_t index, int value)
{
	return setValue(index, ValueType::INT, reinterpret_cast<const std::byte*>(&value));
}


bool
UniformBlock::setValue(size_t index, float value)
{
	return setValue(index, ValueType::FLOAT, reinterpret_cast<const std::byte*>(&value));
}


bool
UniformBlock::setValue(std::string_view name, bool value)
{
	int v = value;
	return setValue(name, ValueType::BOOL, reinterpret_cast<const std::byte*>(&v));
}


bool
UniformBlock::setValue(std::string_view name, int value)
{
	return setValue(name, ValueType::INT, reinterpret_cast<const std::byte*>(&value));
}


bool
UniformBlock::setValue(std::string_view name, float value)
{
	return setValue(name, ValueType::FLOAT, reinterpret_cast<const std::byte*>(&value));
}


bool
UniformBlock::setValue(size_t index, ValueType type, const std::byte* value)
{
	if (index >= mDescription.members.size())
	{
		return false;
	}

	if (type != mDescription.members[index].type)
	{
		return false;
	}

	auto dest = mData.data() + mByteOffsets[index];

	if (type == ValueType::MAT33F)
	{
		constexpr size_t size = byteSize(ValueType::VEC3F);
		constexpr size_t stride = byteSize(ValueType::VEC4F);

		for (size_t i = 0; i < 3; ++i)
		{
			std::memcpy(dest + stride * i, value + size * i, size);
		}
	}
	else
	{
		std::memcpy(dest, value, byteSize(type));
	}

	return true;
}


bool
UniformBlock::setValue(std::string_view name, ValueType type, const std::byte* value)
{
	auto iter = std::find_if(mDescription.members.begin(), mDescription.members.end(),
						     [&](const auto& member) { return member.name == name; });

	if (iter == mDescription.members.end())
	{
		return false;
	}

	auto index = std::distance(mDescription.members.begin(), iter);

	return setValue(index, type, value);
}


const std::span<const std::byte>
UniformBlock::data() const
{
	return std::span<const std::byte>(mData.data(), mData.size());
}


size_t
UniformBlock::size() const
{
	return mData.size();
}