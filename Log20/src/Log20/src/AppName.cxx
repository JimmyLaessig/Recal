module;

#ifdef _WIN32
#	pragma warning(disable:5105)
#	pragma warning(disable:5202)
#	include <Windows.h>
#endif // _WIN32

#ifdef __linux__
#	include <unistd.h>
#endif // __linux__

#include <string>

module AppName;

std::string
Log::getAppName()
{
#ifdef _WIN32
	char buf[1024];
	DWORD size = GetModuleFileNameA(NULL, buf, 1024);

	return std::string(buf, size);
#endif
#ifdef __linux__ 
	char filename[1024];
	ssize_t size = readlink("/proc/self/exe", &buf[0], size);
	return std::string(filename, result);
#endif
}
