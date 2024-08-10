module;

#include <mutex>
#include <filesystem>
#include <fstream>

module Log20.DefaultLogger;

import AppName;

using namespace Log;


DefaultLogger::DefaultLogger()
#ifdef NDEBUG
	: mLevel(Log::Level::INFO)
	, mSourceLocationLogLevel(Log::Level::NEVER)
#else
	: mLevel(Log::Level::DEBUG)
	, mSourceLocationLogLevel(Log::Level::ERROR)
#endif // NDEBUG
	, mLogTimestamp(true)
	, mLogThreadName(true)
{
	std::filesystem::path program = getAppName();
	
	setLogFilePath(program.replace_extension(".log"));
	
	mAsyncTaskQueue.emplace();
}


std::filesystem::path
DefaultLogger::getLogFilePath() const
{
	std::lock_guard lock(mMutex);
	return mLogFilePath;
}


void
DefaultLogger::setLogFilePath(const std::filesystem::path& path)
{
	std::unique_lock lock(mMutex);

	mLogFilePath	= std::filesystem::absolute(path);
	mLogFileStream	= std::make_shared<std::ofstream>(mLogFilePath, std::ofstream::out | std::ofstream::trunc);

	if (!mLogFileStream->good())
	{
		lock.unlock();
		log(Log::Level::ERROR,
			std::source_location::current(),
			std::format("Failed to open log file at '{}'. The program will not be able to write to the log file.", mLogFilePath.string()));
	}
}


Log::Level
DefaultLogger::getLogLevel() const
{
	std::lock_guard lock(mMutex);
	return mLevel;
}


void
DefaultLogger::setLogLevel(Log::Level level)
{
	std::lock_guard lock(mMutex);
	mLevel = level;
}


Log::Level
DefaultLogger::getSourceLocationLogLevel() const
{
	std::lock_guard lock(mMutex);
	return mSourceLocationLogLevel;
}


void
DefaultLogger::setSourceLocationLogLevel(Log::Level level)
{
	std::lock_guard lock(mMutex);
	mSourceLocationLogLevel = level;
}


bool
DefaultLogger::getLogAsync() const
{
	std::lock_guard lock(mMutex);
	return mAsyncTaskQueue.has_value();
}


bool
DefaultLogger::setLogAsync(bool logAsync)
{
	std::lock_guard lock(mMutex);

	if (!logAsync)
	{
		mAsyncTaskQueue.reset();
		return true;
	}

	if (mAsyncTaskQueue)
	{
		return true;
	}

	mAsyncTaskQueue.emplace();

	return true;
}


bool 
DefaultLogger::getLogTimestamp() const
{
	std::lock_guard lock(mMutex);
	return mLogTimestamp;
	return true;
}


void 
DefaultLogger::setLogTimestamp(bool logTimestamp)
{
	std::lock_guard lock(mMutex);
	mLogTimestamp = logTimestamp;
}


bool
DefaultLogger::getLogThreadName() const
{
	std::lock_guard lock(mMutex);
	return mLogThreadName;
}


void
DefaultLogger::setLogThreadName(bool logThreadName)
{
	std::lock_guard lock(mMutex);
	mLogThreadName = logThreadName;
}


void
DefaultLogger::flush()
{
	std::lock_guard lock(mMutex);

	if (mAsyncTaskQueue)
	{
		mAsyncTaskQueue->flush();
	}
}


void
DefaultLogger::log(Log::Level level, 
				   const std::source_location& location, 
				   std::string_view message)
{
	std::lock_guard lock(mMutex);

	auto now = std::chrono::system_clock::now();

	if (mLevel == Log::Level::NEVER || level < mLevel)
	{
		return;
	}
	
	std::ostringstream messageBuilder;

	if (mLogTimestamp)
	{
		messageBuilder << std::format("{:%FT%TZ} | ", now);
	}

	if (mLogThreadName)
	{
		messageBuilder << std::this_thread::get_id() << " | ";
	}

	if (level <= mSourceLocationLogLevel)
	{
#ifdef WIN32
		constexpr auto separator = '\\';
#else
		constexpr auto separator = '/';
#endif // WIN32

		std::string_view filename = location.file_name();
		filename = filename.substr(filename.find_last_of(separator) + 1);
		messageBuilder << filename << ":" << location.line() << " | ";
	}
	
	messageBuilder << toString(level) << " | ";
	messageBuilder << message << std::endl;

	auto msg = messageBuilder.str();
	
	auto task = [logFileStream = mLogFileStream, msg = std::move(msg)]
	{
		std::printf(msg.c_str());
		*logFileStream << msg;
		logFileStream->flush();
	};

	if (mAsyncTaskQueue)
	{
		mAsyncTaskQueue->enqueue(std::move(task));
	}
	else
	{
		task();
	}
}