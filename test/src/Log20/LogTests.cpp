#include <catch2/catch.hpp>

import Log20;

namespace
{

std::string getLastLogMessage()
{
	auto logFle = Log::getLogger().getLogFilePath();
	return "";
}

}


TEST_CASE("Test Default logger")
{
	auto& logger = Log::getLogger();

	CHECK(logger.getLogLevel() == Log::Level::DEBUG);
	CHECK(logger.getSourceLocationLevel() == Log::Level::DEBUG);

	SECTION("Check the default log file")
	{
		auto logFile = logger.getLogFilePath();

		CHECK(logFile.filename() == "Tests.log");

		CHECK(std::filesystem::exists(logFile));

		CHECK(std::filesystem::file_size(logFile) == 0);

		Log::verbose("Verbose log");
		Log::debug("Debug log");
		Log::info("Info log");
		Log::warn("Warning log");
		Log::error("Error log");
	}
}