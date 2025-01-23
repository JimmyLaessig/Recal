#include <Engine/Core/Resource.hpp>


using namespace Reef;


const std::string&
Resource::name() const
{ 
	return mName;
}


void
Resource::setName(std::string_view name)
{
	mName.assign(name.begin(), name.end());
}


size_t
Resource::version() const
{
	mVersion += mOutdated;
	mOutdated = false;
	return mVersion;
}


void
Resource::markOutdated()
{
	mOutdated = true;
}


std::lock_guard<std::mutex>
Resource::lock() const
{
	return std::lock_guard<std::mutex>(mResourceProtection);
}