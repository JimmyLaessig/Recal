module;

#include <glm/glm.hpp>

#include <cstdint>
#include <memory>
#include <string>
#include <string_view>
#include <unordered_map>
#include <variant>
#include <span>

export module Engine.Shader;

//import Reef.Texture;
//
//import Coral.RAII;

export namespace Reef
{

class Shader
{
public:
	
	virtual const std::string& name() const = 0;

};

} // namespace Reef