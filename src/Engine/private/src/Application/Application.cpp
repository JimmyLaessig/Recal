#include <Engine/Application/Application.hpp>

#include <Engine/Application/ApplicationImpl.hpp>

#include <memory>

namespace Reef
{

std::unique_ptr<Application>
createApplication(const CommandLineArguments& arguments)
{
	return ApplicationImpl::create(arguments);
}

} // namespace Reef
