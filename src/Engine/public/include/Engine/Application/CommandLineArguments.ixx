module;

#include <Engine/System.h>

#include <string_view>

export module Engine.Application.CommandLineArguments;


export namespace Reef
{

class ENGINE_API CommandLineArguments
{
public:

	CommandLineArguments() = default;

	CommandLineArguments(size_t argc, char** argv);

	std::string_view get(size_t i) const;

	std::string_view operator[](size_t i) const;

	size_t size() const;

private:

	char** mArgv{ nullptr };

	size_t mArgc{ 0 };
};

} // namespace Reef

