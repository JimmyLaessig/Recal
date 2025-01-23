#ifndef ENGINE_APPLICATION_ENTRYPOINT
#define ENGINE_APPLICATION_ENTRYPOINT

#include <memory>

#include <Engine/Application/Application.hpp>
#include <Engine/Application/CommandLineArguments.hpp>

namespace Reef
{

extern "C++" bool initialize(Reef::Application& app, const CommandLineArguments& commandLineArguments);

} // namespace Reef


int
main(int argc, char** argv)
{
	Reef::CommandLineArguments arguments(static_cast<size_t>(argc), argv);

	auto app = Reef::createApplication(arguments);

	if (!Reef::initialize(*app, arguments))
	{
		return EXIT_FAILURE;
	}

	auto returnCode = app->run();

	return static_cast<int>(returnCode);
}

#endif // !ENGINE_APPLICATION_ENTRYPOINT