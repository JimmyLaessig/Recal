module;

#include <string_view>

module Engine.TypeRegistry;

import Common.CRC32;

using namespace Reef;


bool
TypeRegistry::registerType(const Type& type, const TypeTraits& typeTraits)
{
	auto& registry = instance();

	auto iter = registry.mRegisteredTypes.find(type.uniqueId());

	if (iter != registry.mRegisteredTypes.end())
	{
		// TODO: Log error
		return false;
	}
	registry.mRegisteredTypes[type.uniqueId()] = { type, typeTraits };

	return true;
}


const Type*
TypeRegistry::getType(size_t id)
{
	auto& registry = instance();

	auto iter = registry.mRegisteredTypes.find(id);

	if (iter == registry.mRegisteredTypes.end())
	{
		// TODO: Log error
		return nullptr;
	}
	return &iter->second.type;
}


const Type*
TypeRegistry::getType(std::string_view name)
{
	auto& registry = instance();

	auto iter = registry.mRegisteredTypes.find(Common::crc32(name));

	if (iter == registry.mRegisteredTypes.end())
	{
		// TODO: Log error
		return nullptr;
	}
	return &iter->second.type;
}


ConstructorWrapper
TypeRegistry::constructor(const Type& type)
{
	auto& registry = instance();

	auto iter = registry.mRegisteredTypes.find(type.uniqueId());

	if (iter == registry.mRegisteredTypes.end())
	{
		// TODO: Log error
		return nullptr;
	}
	return iter->second.traits.constructor;
}


UpdateFunctionWrapper
TypeRegistry::updateFunction(const Type& type)
{
	auto& registry = instance();

	auto iter = registry.mRegisteredTypes.find(type.uniqueId());

	if (iter == registry.mRegisteredTypes.end())
	{
		// TODO: Log error
		return nullptr;
	}
	return iter->second.traits.updateFunction;
}