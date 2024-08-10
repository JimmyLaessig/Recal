module;

#include <Engine/System.h>

#include <memory>

export module Engine.Object;

import Engine.Type;

export namespace Reef
{

/// Base class of all serializable polymorphic data types 
class ENGINE_API Object
{
public:

	virtual ~Object() = default;

	/// Get the type of the class
	static const Type& classType()
	{
		return sType;
	}

	/// Get the type of the object
	virtual const Type& type() const
	{
		return classType();
	}

private:

	friend class TypeRegistry;

	constexpr static Type sType = Type::create<Object>();

	/// Register the type and it's traits with the type registry when the class is loaded
	const static bool sTypeRegistered;

}; // class Object


template<typename Derived, typename Base>
requires std::derived_from<Derived, Base>
Derived* downCast(Base* base)
{
	if (Derived::classType().isSubclassOf(base->type()))
	{
		return nullptr;
	}

	return static_cast<Derived*>(base);
}


template<typename Derived, typename Base>
	requires std::derived_from<Derived, Base>
Derived& downCast(Base& base)
{
	if (Derived::classType().isSubclassOf(base->type()))
	{
		return nullptr;
	}

	return static_cast<Derived&>(base);
}

} // export namespace Reef
