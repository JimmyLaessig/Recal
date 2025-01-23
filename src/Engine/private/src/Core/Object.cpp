#include <Engine/Core/Object.hpp>
#include <Engine/Core/TypeRegistry.hpp>

using namespace Reef;

Type Object::sType = TypeRegistry::registerType<Object>();
