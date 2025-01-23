#include <Engine/Core/TypeRegistry.hpp>

#include <string_view>
#include <unordered_map>

#include <Common/CRC32.hpp>

using namespace Reef;


bool
TypeRegistry::registerType(const Type& type, TypeTraits typeTraits)
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


TypeTraits
TypeRegistry::getTypeTraits(const Type& type)
{
	auto& registry = instance();

	auto iter = registry.mRegisteredTypes.find(type.uniqueId());

	if (iter == registry.mRegisteredTypes.end())
	{
		// TODO: Log error
		return {};
	}
	return iter->second.typeTraits;
}
