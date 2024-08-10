module;

#include <cassert>
#include <filesystem>
#include <string_view>
#include <memory>

module Log20.Logger;

import Log20.DefaultLogger;

namespace
{

class NullLogger : public Log::Logger
{
public:

	std::filesystem::path getLogFilePath() const override { return ""; }

	void setLogFilePath(const std::filesystem::path&) override {}

	Log::Level getLogLevel() const override { return Log::Level::INFO; }

	void setLogLevel(Log::Level) override {}

	void setSourceLocationLogLevel(Log::Level) override {}

	Log::Level getSourceLocationLogLevel() const override { return Log::Level::INFO; }

	bool getLogAsync() const override { return false; }

	bool setLogAsync(bool logAsync) override { return false; }

	bool getLogTimestamp() const override { return false; }

	void setLogTimestamp(bool logTimestamp) override {}

	void setLogThreadName(bool logThreadName) override { }

	bool getLogThreadName() const override { return false; }

	void flush() override {}
	 
	void log(Log::Level, const std::source_location&, std::string_view) {}
};

} // namespace

namespace Log
{

std::unique_ptr<Logger> gLogger = nullptr;

void
setLogger(std::unique_ptr<Logger> logger)
{
	gLogger = std::move(logger);

	if (!gLogger)
	{
		gLogger = std::make_unique<::NullLogger>();
	}
}


Logger&
getLogger()
{
	if (!gLogger)
	{
		gLogger = std::make_unique<DefaultLogger>();
	}
	return *gLogger;
}

} // namespace Log
