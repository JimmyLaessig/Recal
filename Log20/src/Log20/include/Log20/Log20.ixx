module;

#include <Log20/System.h>

#include <format>
#include <source_location>

export module Log20;

export import Log20.Logger;
export import Log20.DefaultLogger;


export namespace Log
{

template <typename... Args>
struct LOG20_API LogFormatString
{
public:
	template <typename String>
		requires std::convertible_to<const String&, std::string_view>
	consteval LogFormatString(const String& format,
							  std::source_location location = std::source_location::current())
		: format(format)
		, location(location)
	{} 

	std::format_string<Args...> format;

	std::source_location location;
};

template <class... Args>
using FormatString = LogFormatString<std::type_identity_t<Args>...>;

/// Log a formatted verbose message
/**
 * The message is logged if the logger's log level is equal to or larger than VERBOSE.
 */
template<typename ...Args>
LOG20_API void verbose(const Log::FormatString<Args...> format, Args&&... args)
{
	Log::getLogger().log(Log::Level::VERBOSE, 
						 format.location,
						 std::format(format.format, std::forward<Args&&>(args)...));
}


/// Log a formatted debug message
/**
 * The message is logged if the logger's log level is DEBUG or lower.
 */
template<typename ...Args>
LOG20_API void debug(const FormatString<Args...> format, Args&&... args)
{
	Log::getLogger().log(Log::Level::DEBUG, 
						 format.location,
						 std::format(format.format, std::forward<Args&&>(args)...));
}


/// Log a formatted info message
/**
 * The message is logged if the logger's log level is INFO or lower.
 */
template<typename ...Args>
LOG20_API void info(const FormatString<Args...> format, Args&&... args)
{
	Log::getLogger().log(Log::Level::INFO,
						 format.location,
						 std::format(format.format, std::forward<Args&&>(args)...));
}


/// Log a formatted warning message
/**
 * The message is logged if the logger's log level is WARNING or lower.
 */
template<typename ...Args>
LOG20_API void warn(const FormatString<Args...> format, Args&&... args)
{
	Log::getLogger().log(Log::Level::WARNING,
						 format.location,
						 std::format(format.format, std::forward<Args&&>(args)...));
}


/// Log a formatted error message
/**
 * The message is logged if the logger's log level is ERROR or lower.
 */
template<typename ...Args>
LOG20_API void error(const FormatString<Args...> format, Args&&... args)
{
	Log::getLogger().log(Log::Level::ERROR,
						 format.location,
						 std::format(format.format, std::forward<Args&&>(args)...));
}

} // namespace Log
