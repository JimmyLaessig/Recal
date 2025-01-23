#ifndef COMMON_NONCOPYABLE_HPP
#define COMMON_NONCOPYABLE_HPP

#include <Common/System.hpp>

namespace Common
{

/// Derive from this class to disable copy capabilities of the derived class.
class COMMON_API NonCopyable
{
public:

	NonCopyable() = default;

	NonCopyable(const NonCopyable&) = delete;

	NonCopyable& operator=(const NonCopyable&) = delete;

};

} // export namespace Common

#endif // !COMMON_NONCOPYABLE_HPP
