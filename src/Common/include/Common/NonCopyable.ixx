module;

#include <Common/System.h>

export module Common.NonCopyable;

export namespace Common
{

/// Derive from this class to disable copy capabilities of the derived class.
class COMMON_API NonCopyable
{
public:

	NonCopyable() = default;

	NonCopyable(const NonCopyable&) = delete;

	NonCopyable& operator=(const NonCopyable&) = delete;

}; // class NonCopyable

} // export namespace Common
