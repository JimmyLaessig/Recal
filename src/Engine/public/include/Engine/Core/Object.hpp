#ifndef ENGINE_CORE_OBJECT_HPP
#define ENGINE_CORE_OBJECT_HPP

#include <Engine/System.hpp>

#include <Engine/Core/Type.hpp>

#include <memory>

namespace Reef
{

/// Base class of all serializable polymorphic data types 
class ENGINE_API Object
{
public:

	Object() = default;

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

	static Type sType;

};


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

} // namespace Reef

#endif // !ENGINE_CORE_OBJECT_HPP
