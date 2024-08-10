module;

#include <Engine/System.h>

#include <unordered_map>
#include <string_view>
#include <memory>
#include <concepts>

export module Engine.TypeRegistry;

import Engine.Object;
import Engine.Type;
import Engine.TypeTraits;

import Singleton;

export namespace Reef
{

/// Engine-wide registry that stores the type information of all constructible types
class ENGINE_API TypeRegistry : public Common::Singleton<TypeRegistry>
{
public:

	/// Get the type based on the type id
	static const Type* getType(size_t id);

	/// Get the type based on the type name
	static const Type* getType(std::string_view name);

	static ConstructorWrapper constructor(const Type& type);

	static UpdateFunctionWrapper updateFunction(const Type& type);

	template<typename T>
	static inline bool registerType()
	{
		return registerType(T::classType(), TypeTraits::create<T>());
	}

private:

	/// Register a new type with it's traits
	static bool registerType(const Type& type, const TypeTraits& typeTraits);

	struct PerTypeData
	{
		Type type;
		TypeTraits traits;
	};

	std::unordered_map<size_t, PerTypeData> mRegisteredTypes;

}; // class TypeRegistry

} // export namespace Reef
