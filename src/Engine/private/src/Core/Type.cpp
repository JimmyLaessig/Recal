#include <Engine/Core/Type.hpp>
#include <Engine/Core/TypeRegistry.hpp>

#include <array>
#include <cstdint>
#include <string_view>

using namespace Reef;

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