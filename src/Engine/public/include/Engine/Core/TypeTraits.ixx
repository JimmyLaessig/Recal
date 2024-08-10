module;

#include <Engine/System.h>

#include <array>
#include <concepts>
#include <memory>
#include <string_view>

export module Engine.TypeTraits;

import Engine.Object;

export namespace Reef
{

template<typename T>
concept HasUpdateFunction = requires(T t)
{
	{ t.onUpdate() } -> std::convertible_to<void>;
};

using ConstructorWrapper = std::unique_ptr<Object>(*)(void);


using UpdateFunctionWrapper = void(*)(Object&);


struct ENGINE_API TypeTraits
{
	UpdateFunctionWrapper updateFunction{ nullptr };

	ConstructorWrapper constructor{ nullptr };

	template <typename T>
	static TypeTraits create();
};


} // export namespace Reef


template<typename T>
Reef::TypeTraits
Reef::TypeTraits::create()
{
	Reef::TypeTraits typeTraits;

	if constexpr (!std::is_abstract_v<T>)
	{
		typeTraits.constructor = []() { return std::unique_ptr<Object>(static_cast<Object*>(new T)); };
	}

	if constexpr (HasUpdateFunction<T>)
	{
		typeTraits.updateFunction = [](Object& obj) -> void { return static_cast<T&>(obj).onUpdate(); };
	}

	return typeTraits;
}
