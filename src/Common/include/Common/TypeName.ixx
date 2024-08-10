module;

#include <Common/System.h>

#include <string_view>

export module Common.TypeName;

export namespace Common
{

/// Get the type name of the template argument.
template<typename Class>
constexpr COMMON_API std::string_view typeName()
{
#ifdef  _MSC_VER
	// MSVC predefines the '__FUNCSIG__' preprocessor definition, which, for the templated function, contains the name
	// of the template argument.
	constexpr std::string_view functionName{ __FUNCSIG__ };
	constexpr std::string_view prefix{ "Common::typeName<" };
	constexpr std::string_view suffix{ ">()" };
#else //  _MSC_VER
	static_assert(false && "Unsupported compiler");
#endif

	constexpr auto left = functionName.find(prefix);
	constexpr auto right = functionName.rfind(suffix);

	static_assert(left < right);

	auto start = left + suffix.size();
	auto length = right - start;

	return functionName.substr(start, length);
}

} // namespace Common