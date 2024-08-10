module;

#include <array>
#include <cstdint>
#include <string_view>

module Engine.Type;

import Engine.TypeRegistry;

using namespace Reef;

constexpr const
std::string_view Type::name() const
{
	return mName;
}


constexpr size_t
Type::uniqueId() const
{
	return mUniqueId;
}


const Type*
Type::parent() const
{
	if (mHasParent)
	{
		return TypeRegistry::instance().getType(mParentUniqueId);
	}

	return nullptr;
}


bool
Type::isSubclassOf(const Type& other) const
{
	for (auto t = this; t; t = t->parent())
	{
		if (*t == other)
		{
			return true;
		}
	}

	return false;
}


bool
Type::operator==(const Type& other) const
{
	return mUniqueId == other.mUniqueId;
}