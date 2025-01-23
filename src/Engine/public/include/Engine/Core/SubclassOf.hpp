#ifndef ENGINE_CORE_SUBCLASSOF_HPP
#define ENGINE_CORE_SUBCLASSOF_HPP

#include <Engine/System.hpp>

#include <Engine/Core/Object.hpp>
#include <Engine/Core/Type.hpp>
#include <Engine/Core/TypeRegistry.hpp>

#include <concepts>
#include <memory>


namespace Reef
{

template<typename Derived, typename Base>
requires std::derived_from<Base, Object>
class ENGINE_API SubclassOf : public Base
{
public:

	using Base::Base;

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

	static inline Type sType{ TypeRegistry::registerType<Derived, Base>() };
};

} // namespace Reef

#endif // !ENGINE_CORE_SUBCLASSOF_HPP
