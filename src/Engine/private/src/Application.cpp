module;

#include <memory>

module Engine.Application;

import Engine.Application.ApplicationImpl;

using namespace Reef;

std::unique_ptr<Application>
Application::create(int argc, char** argv)
{
	return std::unique_ptr<Application>(new ApplicationImpl);
}

