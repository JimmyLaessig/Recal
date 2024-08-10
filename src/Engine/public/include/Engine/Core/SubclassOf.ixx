module;

#include <Engine/System.h>

#include <concepts>
#include <type_traits>
#include <memory>

#include <concepts>
#include <iostream>

export module Engine.SubclassOf;

import Engine.Object;
import Engine.Type;
import Engine.TypeRegistry;

export namespace Reef
{

template<typename Derived, typename Base>
requires std::derived_from<Base, Object>
class ENGINE_API SubclassOf : public Base
{
public:

	/// Get the type of the class
	static const Type& classType()
	{
		return sType;
	}

	/// Get the type of the object
	virtual const Type& type() const override
	{
		return classType();
	}

private:

	constexpr static Type sType{ Type::create<Derived, Base>() };
	/// Register the type and it's traits with the type registry when the class is loaded
	const static inline bool sTypeRegistered{ TypeRegistry::registerType<Derived>() };
};

} // export namespace Reef
