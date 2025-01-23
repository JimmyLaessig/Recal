
#include <MaterialGenerator/MaterialGenerator.h>


#include <optional>
//#include <span>
//#include <unordered_map>
//#include <array>
//#include <string_view>
//#include <algorithm>
//#include <fstream>
//#include <sstream>
//#include <filesystem>
//#include <iostream>
//#include <functional>
//#include <cassert>
//#include <sstream>
//#include <ranges>

namespace
{
std::vector<std::pair<std::string, std::string>>
getPreprocessorDefinitions(const std::string& shaderSource)
{

}

} // namespace

namespace Reef
{

std::optional<GeneratedMaterialInfo>
generateMaterial(const MaterialCreateInfo& program)
{
	auto defines = getPreprocessorDefinitions(program.materialSource);


	return {};
}

} // namespace Reef