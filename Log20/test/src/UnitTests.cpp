#include <catch2/catch_all.hpp>

#include <fstream>
#include <filesystem>


import Log20;
import Log20.DefaultLogger;

namespace
{

struct LogMessage
{
	std::string timestamp;
	std::string threadId;
	std::string location;
	Log::Level level;
	std::string message;
};


std::vector<LogMessage>
getLogs(const std::filesystem::path& logFilePath, bool withTimestamp, bool withThreadId)
{
	std::ifstream fs(logFilePath.c_str(), std::fstream::in);
	REQUIRE(fs.good());

	std::string lastLine;
	std::vector<LogMessage> logs;

	for (std::string line; std::getline(fs, line);)
	{
		std::stringstream stream(line);
		std::vector<std::string> args;

		// Log message parts are separated by a vertical line
		for (std::string argument; std::getline(stream, argument, '|');)
		{
			std::string cleaned(argument.begin() + argument.starts_with(' '), argument.end() - argument.ends_with(' '));
			args.push_back(cleaned);
		}

		LogMessage message;

		// Timestamp is the first argument of the log message
		if (withTimestamp)
		{
			message.timestamp = args[0];
		}

		// ThreadId is the second argument of the log message
		if (withThreadId)
		{
			message.threadId = args[withTimestamp];
		}

		// The (optional) source location is the third argument
		if (args.size() + !withTimestamp + !withThreadId == 5)
		{
			message.location = args[withTimestamp + withThreadId];
		}
		
		// The second-to-last argument is the log level
		message.level = Log::toLogLevel(args[args.size() - 2]);
		// The last argument is the log message
		message.message = args[args.size() - 1];

		logs.push_back(std::move(message));
	}

	return logs;
}


class TestLogger : public Log::Logger
{
public:

	std::filesystem::path getLogFilePath() const override { return ""; }

	void setLogFilePath(const std::filesystem::path& path) override {}

	Log::Level getLogLevel() const override { return Log::Level::INFO; }

	void setLogLevel(Log::Level level)  override {}

	void setSourceLocationLogLevel(Log::Level level) override {}

	Log::Level getSourceLocationLogLevel() const override { return Log::Level::INFO; }

	bool getLogAsync() const  override { return true; }

	bool setLogAsync(bool logAsync)  override { return true; }

	bool getLogTimestamp() const  override { return true; }

	void setLogTimestamp(bool logTimestamp)  override {}

	void setLogThreadName(bool logThreadName)  override {}

	bool getLogThreadName() const  override { return true; }

	void flush() override {}

	void log(Log::Level level, const std::source_location& location, std::string_view message) override
	{
		mNumLogCalls++;
	}

	uint32_t mNumLogCalls{ 0 };
};


void
checkLogMessage(const LogMessage& message, 
				Log::Level expectedLevel, 
				const std::string& expectedMessage, 
				bool expectTimestamp, 
				bool expectThreadId)
{
	CHECK(message.level == expectedLevel);
	CHECK(message.message == expectedMessage);
	CHECK(message.timestamp.empty() != expectTimestamp);
	CHECK(message.threadId.empty() != expectThreadId);
}

} // namespace


TEST_CASE("Test Default logger")
{
	Log::setLogger(std::make_unique<Log::DefaultLogger>());
	auto& logger = Log::getLogger();

	SECTION("Test default properties")
	{
		CHECK(logger.getLogFilePath().filename() == "Log20.Tests.log");

#ifdef NDEBUG
		CHECK(logger.getLogLevel() == Log::Level::INFO);
		CHECK(logger.getSourceLocationLogLevel() == Log::Level::NEVER);
#else
		CHECK(logger.getLogLevel() == Log::Level::DEBUG);
		CHECK(logger.getSourceLocationLogLevel() == Log::Level::ERROR);
#endif // NDEBUG

		CHECK(logger.getLogAsync());
		CHECK(logger.getLogThreadName());
		CHECK(logger.getLogTimestamp());
	}

	auto logAsync = GENERATE(true, false);
	logger.setLogAsync(logAsync);

	bool defaultLogFile = GENERATE(true, false);
	if (!defaultLogFile)
	{
		auto otherLogFilePath = logger.getLogFilePath().replace_filename("OtherLogFile.log");

		CHECK_FALSE(logger.getLogFilePath() == otherLogFilePath);

		logger.setLogFilePath(otherLogFilePath);
		CHECK(logger.getLogFilePath() == otherLogFilePath);
		CHECK(std::filesystem::exists(otherLogFilePath));
		CHECK(std::filesystem::is_empty(otherLogFilePath));
	}

	SECTION("Test different log levels")
	{
		auto logTimestamp = GENERATE(false, true);
		logger.setLogTimestamp(logTimestamp);

		auto logThreadId = GENERATE(false, true);
		logger.setLogThreadName(logThreadId);

		auto logLevel = GENERATE(Log::Level::NEVER, 
								 Log::Level::VERBOSE,
								 Log::Level::DEBUG, 
								 Log::Level::INFO, 
								 Log::Level::WARNING, 
								 Log::Level::ERROR);

		logger.setLogLevel(logLevel);

		Log::verbose("Verbose log {}", 1);
		Log::debug("Debug log {}", 'x');
		Log::info("Info log {}", "test");
		Log::warn("Warn log {}", 0.5f);
		Log::error("Error log");

		logger.flush();
		auto logs = getLogs(logger.getLogFilePath(), logTimestamp, logThreadId);

		uint32_t i{ 0 };
		switch (logLevel)
		{
			case Log::Level::VERBOSE:
				checkLogMessage(logs[i++], Log::Level::VERBOSE, "Verbose log 1", logTimestamp, logThreadId);
			case Log::Level::DEBUG:
				checkLogMessage(logs[i++], Log::Level::DEBUG, "Debug log x", logTimestamp, logThreadId);
			case Log::Level::INFO:
				checkLogMessage(logs[i++], Log::Level::INFO, "Info log test", logTimestamp, logThreadId);
			case Log::Level::WARNING:
				checkLogMessage(logs[i++], Log::Level::WARNING, "Warn log 0.5", logTimestamp, logThreadId);
			case Log::Level::ERROR: 
				checkLogMessage(logs[i++], Log::Level::ERROR, "Error log", logTimestamp, logThreadId);
				CHECK(logs.size() == i);
				break;
			case Log::Level::NEVER:
				CHECK(logs.empty());
		}
	}

	SECTION("Test different source location level")
	{
		auto sourceLocationLogLevel = GENERATE(Log::Level::VERBOSE, Log::Level::DEBUG, Log::Level::INFO, Log::Level::WARNING, Log::Level::ERROR);
		logger.setSourceLocationLogLevel(sourceLocationLogLevel);
		logger.setLogLevel(Log::Level::VERBOSE);

		Log::verbose("Verbose log {}", 1);
		Log::debug("Debug log {}", 'x');
		Log::info("Info log {}", "test");
		Log::warn("Warn log {}", 0.5f);
		Log::error("Error log");

		logger.flush();
		auto logs = getLogs(logger.getLogFilePath(), true, true);

		REQUIRE(logs.size() == 5);
		for (auto& log: logs)
		{
			if (log.level > sourceLocationLogLevel)
			{
				CHECK(log.location.empty());
			}
			else
			{
				CHECK(log.location.starts_with("UnitTests.cpp:"));
			}
		}
	}
}


TEST_CASE("Test Null logger")
{
	Log::setLogger(std::make_unique<Log::DefaultLogger>());
	auto logFilePath = Log::getLogger().getLogFilePath();
	Log::setLogger(nullptr);

	CHECK(Log::getLogger().getLogFilePath() == "");
	CHECK(std::filesystem::is_empty(logFilePath));
	Log::verbose("Verbose message");
	CHECK(std::filesystem::is_empty(logFilePath));
	Log::debug("Debug message");
	CHECK(std::filesystem::is_empty(logFilePath));
	Log::info("Info message");
	CHECK(std::filesystem::is_empty(logFilePath));
	Log::warn("Warn message");
	CHECK(std::filesystem::is_empty(logFilePath));
	Log::error("Error message");
	CHECK(std::filesystem::is_empty(logFilePath));
}


TEST_CASE("Test Custom Logger")
{
	// Set the logger to our own implementation
	Log::setLogger(std::make_unique<TestLogger>());

	auto& testLogger = static_cast<TestLogger&>(Log::getLogger());

	CHECK(testLogger.mNumLogCalls == 0);
	Log::info("First message");
	CHECK(testLogger.mNumLogCalls == 1);
	Log::info("Second message");
	CHECK(testLogger.mNumLogCalls == 2);
}
