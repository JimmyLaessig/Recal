module;

#include <Common/System.h>

export module Singleton;

import Common.NonCopyable;

export namespace Common
{

/// Base class for a Singleton
template<typename T>
class COMMON_API Singleton : Common::NonCopyable
{
public:

	static T& instance();

protected:

	Singleton() = default;

}; // class Singleton

} // export namespace Common

template<typename T>
T& Common::Singleton<T>::instance()
{
	static T sInstance;

	return sInstance;
}