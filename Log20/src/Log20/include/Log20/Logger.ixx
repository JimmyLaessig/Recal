module;

#include <Log20/System.h>

#include <filesystem>
#include <string_view>
#include <source_location>
#include <memory>

export module Log20.Logger;

export namespace Log
{

enum class Level
{
	NEVER		= 0,
	VERBOSE		= 1,
	DEBUG		= 2,
	INFO		= 3,
	WARNING		= 4,
	ERROR		= 5,
};


constexpr std::string_view
toString(Log::Level level)
{
	switch (level)
	{
		case Log::Level::NEVER:		return "NEVER";
		case Log::Level::VERBOSE:	return "VERBOSE";
		case Log::Level::DEBUG:		return "DEBUG";
		case Log::Level::INFO:		return "INFO";
		case Log::Level::WARNING:	return "WARNING";
		case Log::Level::ERROR:		return "ERROR";
		default:					return "";
	}
}


constexpr Log::Level
toLogLevel(std::string_view level)
{
	if (level == "NEVER")
		return Log::Level::NEVER;
	if (level == "VERBOSE")
		return Log::Level::VERBOSE;
	if (level == "DEBUG")
		return Log::Level::DEBUG;
	if (level == "INFO")
		return Log::Level::INFO;
	if (level == "WARNING")
		return Log::Level::WARNING;
	if (level == "ERROR")
		return Log::Level::ERROR;

	return Log::Level::NEVER;
}


class LOG20_API Logger
{
public:

	virtual ~Logger() = default;

	/// Get the path to the log file
	virtual std::filesystem::path getLogFilePath() const = 0;

	/// Set the path to the log file
	virtual void setLogFilePath(const std::filesystem::path& path) = 0;

	/// Get the current log level
	/**
	 * Messages with log level lower than the current log level will not be logged.
	 */
	virtual Level getLogLevel() const = 0;

	/// Set the current log level
	/**
	 * Messages with log level lower than the current log level will not be logged.
	 */
	virtual void setLogLevel(Level level) = 0;

	/// Set the current source location log level
	/**
	 * Messages with this log level or lower will have their source locatin logged.
	 */
	virtual void setSourceLocationLogLevel(Level level) = 0;

	/// Set the current source location log level
	/**
	 * Messages with this log level or lower will have their source locatin logged.
	 */
	virtual Level getSourceLocationLogLevel() const = 0;

	/// Flag indicating if the logger should use a separate logging thread for log-message processing
	virtual bool getLogAsync() const = 0;

	/// Set if the logger should use a separate logging thread for log-message processing
	virtual bool setLogAsync(bool logAsync) = 0;

	/// Flag indicating if the timestamp should be part of the log messages
	virtual bool getLogTimestamp() const = 0;

	/// Set if the timestamp should be part of the log messages
	virtual void setLogTimestamp(bool logTimestamp) = 0;

	/// Flag indicating if the thread name should be part of the log messages
	virtual void setLogThreadName(bool logThreadName) = 0;

	/// Set if the thread name should be part of the log messages
	virtual bool getLogThreadName() const = 0;

	/// Flushes all queued log messages to the output stream.
	virtual void flush() = 0;

	/// Log the message
	virtual void log(Level level, const std::source_location& location, std::string_view message) = 0;
};

/// Set logger
/**
* \note: This function is not thread-safe.
*/
LOG20_API void setLogger(std::unique_ptr<Logger> logger);

/// Get the logger
/**
 * \note: This function is not thread-safe.
 */
LOG20_API Logger& getLogger();

} // namespace Log
