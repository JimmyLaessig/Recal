#ifndef COMMON_TYPENAME_HPP
#define COMMON_TYPENAME_HPP

#include <string_view>

namespace Common
{

namespace Detail
{

template<typename T>
struct TypeNameImpl
{
	constexpr std::string_view operator()()
	{
#ifdef  _MSC_VER
		// MSVC predefines the '__FUNCSIG__' preprocessor definition, which, for the templated function, contains the
		// name of the template argument.

		constexpr std::string_view functionName{ __FUNCSIG__ };
		constexpr std::string_view prefix{ "Common::Detail::TypeNameImpl<" };
		constexpr std::string_view suffix{ ">::operator ()(void)" };

		constexpr auto left = functionName.find(prefix) + prefix.size();
		constexpr auto right = functionName.rfind(suffix);

		static_assert(left < right);

		constexpr auto name = functionName.substr(left, right - left);

		constexpr std::string_view classPrefix{ "class " };
		if (name.starts_with(classPrefix))
		{
			return name.substr(classPrefix.size());
		}

		constexpr std::string_view structPrefix{ "struct " };
		if (name.starts_with(structPrefix))
		{
			return name.substr(structPrefix.size());
		}

		return name;

#else //  _MSC_VER
		static_assert(false && "Unsupported compiler");
#endif
	}
};


template<template<typename, typename...> class T, typename U, typename... Rest>
struct TypeNameImpl<T<U, Rest...>>
{
	constexpr std::string_view operator()()
	{
		static_assert(false && "templated types are not supported");
		return "";
	}
};

} // namespace Detail

/// Get the name of the type T as a string at compile time
/**
 * \Note: Templated types are not supported. Furthermore, This function does not work for aliased types and will return
 * the non-aliased type name instead.
 */
template<typename T>
constexpr std::string_view typeName()
{
	return Detail::TypeNameImpl<T>()();
}

} // namespace Common

#endif // !COMMON_TYPENAME_HPP