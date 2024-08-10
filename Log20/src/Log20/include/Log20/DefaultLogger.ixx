module;

#include <Log20/System.h>

#include <filesystem>
#include <fstream>
#include <memory>
#include <mutex>
#include <optional>
#include <source_location>
#include <string_view>

export module Log20.DefaultLogger;

import Log20.Logger;
import AsyncTaskQueue;

export namespace Log
{

class LOG20_API DefaultLogger : public Logger
{
public:

	DefaultLogger();

	~DefaultLogger()
	{

	}

	/// Get the path to the log file
	std::filesystem::path getLogFilePath() const override;

	/// Set the path to the log file
	void setLogFilePath(const std::filesystem::path& path) override;

	/// Get the current log level
	/**
	 * Messages with log level lower than the current log level will not be logged.
	 */
	Level getLogLevel() const override;

	/// Set the current log level
	/**
	 * Messages with log level lower than the current log level will not be logged.
	 */
	void setLogLevel(Level level) override;

	/// Set the current source location log level
	/**
	 * Messages with this log level or lower will have their source locatin logged.
	 */
	void setSourceLocationLogLevel(Level level) override;

	/// Set the current source location log level
	/**
	 * Messages with this log level or lower will have their source locatin logged.
	 */
	Level getSourceLocationLogLevel() const override;

	/// Flag indicating if the logger should use a separate logging thread for log-message processing
	bool getLogAsync() const override;

	/// Set if the logger should use a separate logging thread for log-message processing
	bool setLogAsync(bool logAsync) override;

	/// Flag indicating if the timestamp should be part of the log messages
	bool getLogTimestamp() const override;

	/// Set if the timestamp should be part of the log messages
	void setLogTimestamp(bool logTimestamp) override;

	/// Flag indicating if the thread name should be part of the log messages
	void setLogThreadName(bool logThreadName) override;

	/// Set if the thread name should be part of the log messages
	bool getLogThreadName() const override;

	/// Flushes all queued log messages to the output stream.
	void flush() override;

	/// Log the message
	void log(Level level, const std::source_location& location, std::string_view message) override;

private:

	/// The log level at which messages are logged
	Log::Level mLevel;

	/// The log level at which the source location should be logged
	Log::Level mSourceLocationLogLevel;

	/// Flag indicating if the timestamp should be included in the log messages
	bool mLogTimestamp;

	/// Flag indicating if the thread name should be included in the log messages
	bool mLogThreadName;

	/// The path to the log file
	std::filesystem::path mLogFilePath;

	/// The file stream of the log file
	std::shared_ptr<std::ofstream> mLogFileStream;

	/// Thread-safety protection
	mutable std::mutex mMutex;

	/// In async mode, log messages are processed in a queue a separate thread
	std::optional<AsyncTaskQueue> mAsyncTaskQueue;
};

} // namespace Log
