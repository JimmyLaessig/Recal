#ifndef COMMON_SMARTPOINTER_HPP
#define COMMON_SMARTPOINTER_HPP

#include <Common/System.hpp>

#include <memory>

namespace Common
{

/// Cast the object of type T to an object of type U
template<typename T, typename U>
[[nodiscard]] std::unique_ptr<T>
COMMON_API
staticPointerCast(std::unique_ptr<U>&& ptr)
{
	return std::unique_ptr<T>(static_cast<T*>(ptr.release()));
}


/// Cast the object of type T to an object of type U
template<typename T, typename U>
[[nodiscard]] std::unique_ptr<T>
COMMON_API
reinterpretPointerCast(std::unique_ptr<U>&& ptr)
{
	return std::unique_ptr<T>(reinterpret_cast<T*>(ptr.release()));
}

} // namespace Common

#endif // !COMMON_SMARTPOINTER_HPP