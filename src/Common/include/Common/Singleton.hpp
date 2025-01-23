#ifndef COMMON_SINGLETON_HPP
#define COMMON_SINGLETON_HPP

#include <Common/System.hpp>

#include <Common/NonCopyable.hpp>

namespace Common
{

/// Base class for a Singleton
template<typename T>
class COMMON_API Singleton : Common::NonCopyable
{
public:

	static T& instance()
	{
		static T sInstance;

		return sInstance;
	}

protected:

	Singleton() = default;

};

} // namespace Common

#endif // !COMMON_SINGLETON_HPP