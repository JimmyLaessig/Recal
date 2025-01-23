#ifndef ENGINE_CORE_TYPETRAITS_HPP
#define ENGINE_CORE_TYPETRAITS_HPP

#include <Engine/Core/Object.hpp>

#include <Common/SmartPointer.hpp>

#include <array>
#include <concepts>
#include <memory>
#include <string_view>
#include <iostream>

namespace Reef
{

template<typename T>
concept HasOnUpdateFunction = requires(T t)
{
	{ t.onUpdate() } -> std::convertible_to<void>;
};

using ConstructorDelegate = std::unique_ptr<Object>(*)();

using OnUpdateDelegate = void(*)(Object&);

struct ENGINE_API TypeTraits
{
	OnUpdateDelegate onUpdateDelegate{ nullptr };

	ConstructorDelegate constructorDelegate{ nullptr };

	template <typename T>
	static TypeTraits create();
};

} // namespace Reef


template<typename T>
Reef::TypeTraits
Reef::TypeTraits::create()
{
	Reef::TypeTraits typeTraits;

	if constexpr (!std::is_abstract_v<T> && std::is_default_constructible_v<T>)
	{
		typeTraits.constructorDelegate = []() 
		{
			return std::unique_ptr<Object>(new T);
		};
	}

	if constexpr (HasOnUpdateFunction<T>)
	{
		typeTraits.onUpdateDelegate = [](Object& obj) -> void { return static_cast<T&>(obj).onUpdate(); };
	}

	return typeTraits;
}

#endif // !ENGINE_CORE_TYPETRAITS_HPP