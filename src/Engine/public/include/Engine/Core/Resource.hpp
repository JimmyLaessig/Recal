#ifndef ENGINE_CORE_RESOURCE_HPP
#define ENGINE_CORE_RESOURCE_HPP

#include <Engine/Core/SubclassOf.hpp>

#include <cassert>
#include <memory>
#include <span>
#include <string>
#include <string_view>
#include <vector>
#include <mutex>

namespace Reef
{

class ENGINE_API ResourceLock;

class ENGINE_API Resource : public SubclassOf<Resource, Object>
{
public:

	virtual ~Resource() = default;

	/// Get the name of the resource
	const std::string& name() const;

	/// Set the name of the resource
	/*
	 * \Note: This function is not thread-safe and should only be called from within a locked scope.
	 */
	void setName(std::string_view name);

	/// Get the version of the resource
	/*
	 * \Note: This function is not thread-safe and should only be called from within a locked scope.
	 */
	size_t version() const;

	/// Lock the resource from concurrent access
	/**
	 * \Note: The resource lock must be destroyed before the resource object itself is destroyed.
	 */
	[[nodiscard]] std::lock_guard<std::mutex> lock() const;

protected:

	/// Mark the resource outdated
	/**
	 * Calling this function bump the version number so that consumers of the resource are notified that the resource
	 * has changed.
	 * \Note: This function is not thread-safe and should only be called from within a locked scope.
	 */
	void markOutdated();

private:

	std::string mName;

	mutable bool mOutdated{ false };

	mutable size_t mVersion{ 0 };

	mutable std::mutex mResourceProtection;
};



} // namespace Reef

#endif // !ENGINE_CORE_RESOURCE_HPP