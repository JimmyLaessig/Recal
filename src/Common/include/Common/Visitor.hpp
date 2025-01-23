#ifndef COMMON_VISITOR_HPP
#define COMMON_VISITOR_HPP

#include <Common/System.hpp>

#include <variant>

namespace Common
{

// Visitor class definition for std::visit
template<class... Callables>
struct COMMON_API Visitor : Callables...
{ 
	using Callables::operator()...;
};


template<typename Variant, typename ...Callables>
auto visit(Variant&& variant, Visitor<Callables...>&& visitor) -> decltype(auto)
{
	return std::visit(visitor, variant);
}

} // export namespace Common

#endif //!COMMON_VISITOR_HPP