#ifndef ENGINE_CORE_TYPEREGISTRY_HPP
#define ENGINE_CORE_TYPEREGISTRY_HPP

#include <Engine/System.hpp>

#include <Engine/Core/Object.hpp>
#include <Engine/Core/Type.hpp>
#include <Engine/Core/TypeTraits.hpp>

#include <Common/Singleton.hpp>
#include <Common/TypeName.hpp>

#include <unordered_map>
#include <string_view>
#include <memory>
#include <concepts>

namespace Reef
{

/// Engine-wide registry to store the type information of all constructible types
class ENGINE_API TypeRegistry : public Common::Singleton<TypeRegistry>
{
public:

	/// Get the type based on the type id
	static const Type* getType(size_t id);

	/// Get the type based on the type name
	static const Type* getType(std::string_view name);

	/// Get the type trais of the given type
	static TypeTraits getTypeTraits(const Type& type);

	template<typename Class>
	static inline Type registerType()
	{
		Type type(Common::typeName<Class>());
		registerType(type, TypeTraits::create<Class>());
		return type;
	}

	template<typename Class, typename BaseClass>
	static inline Type registerType()
	{
		Type type(Common::typeName<Class>(), Common::typeName<BaseClass>());
		registerType(type, TypeTraits::create<Class>());

		return type;
	}

private:

	/// Register a new type with it's constructor
	static bool registerType(const Type& type, TypeTraits typeTraits);

	struct PerTypeData
	{
		Type type;
		TypeTraits typeTraits;
	};

	std::unordered_map<size_t, PerTypeData> mRegisteredTypes;

}; 

} // namespace Reef

#endif // !ENGINE_CORE_TYPEREGISTRY_HPP
