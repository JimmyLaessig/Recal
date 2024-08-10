module;

#include <memory>

module Engine.Application;

import Engine.Application.ApplicationImpl;


namespace Reef
{

std::unique_ptr<IApplication>
createApplication(const CommandLineArguments& arguments)
{
	return ApplicationImpl::create(arguments);
}

} // namespace Reef
