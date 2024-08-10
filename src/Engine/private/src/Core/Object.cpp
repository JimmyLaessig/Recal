module;

#include <memory>

module Engine.Object;

import Engine.TypeRegistry;

using namespace Reef;

const bool Object::sTypeRegistered = TypeRegistry::registerType<Object>();