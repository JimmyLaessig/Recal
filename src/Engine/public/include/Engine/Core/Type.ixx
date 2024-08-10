module;

#include <Engine/System.h>

#include <string_view>
#include <utility>

export module Engine.Type;

import Common.CRC32;
import Common.TypeName;

export namespace Reef
{

/// The type class provides runtime access to name and inheritance information of classes and structs without the need
/// for RTTI enabled.
/**
 * Type information must be constructed from the TypeFactory.
 */
class ENGINE_API Type
{
public:

	constexpr Type() = default;

	constexpr Type(std::string_view className);

	constexpr Type(std::string_view className, std::string_view parentClassName);

	/// The name of the type
	constexpr const std::string_view name() const { return mName; }

	/// The unique id of the type
	constexpr size_t uniqueId() const { return mUniqueId; }

	/// Check if this type is a subclass of the other type.
	bool isSubclassOf(const Type& other) const;

	/// Get the parent type of this type if the type inherits from another type or nullptr if the type has no parent
	/// type.
	const Type* parent() const;

	template<typename Class>
	constexpr static Type create();

	template<typename Class, typename BaseClass>
	constexpr static Type create();

	bool operator==(const Type& other) const;

private:

	friend class TypeRegistry;

	/// The class name of the type
	std::string_view mName;

	/// Unique id of the type
	size_t mUniqueId{ 0 };

	/// Unique id of the parent type
	size_t mParentUniqueId{ 0 };

	/// Flag indicating whether or not the class of this type derives from a different class within the type system.
	bool mHasParent{ false };
};

} // export namespace Reef


constexpr
Reef::Type::Type(std::string_view className)
	: mName(className)
	, mUniqueId(Common::crc32(className))
	, mHasParent(false)
{
}


constexpr
Reef::Type::Type(std::string_view className, std::string_view parentClassName)
	: Type(className)
{
	mParentUniqueId = Common::crc32(parentClassName); 
	mHasParent = true;
}


template<typename Class>
constexpr Reef::Type
Reef::Type::create()
{
	return { Common::typeName<Class>() };
}


template<typename Class, typename BaseClass>
constexpr Reef::Type
Reef::Type::create()
{
	return { Common::typeName<Class>(), Common::typeName<BaseClass>() };
}


namespace std
{

export
template<>
struct hash<Reef::Type>
{
	size_t operator()(const Reef::Type& type) noexcept
	{
		return type.uniqueId();
	}
};

} // namespace std

