module;

#include <Common/System.h>

#include <variant>

export module Common.Visitor;

export namespace Common
{

// Visitor class definition for std::visit
template<class... Ts>
struct COMMON_API Visitor : Ts...
{ 
	using Ts::operator()...;
};


//template<class... Ts>
//Visitor(Ts...) -> Visitor<Ts...>;


template<typename Variant, typename ...Ts>
void visit(Variant variant, Visitor<Ts...>&& visitor)
{
	return std::visit(visitor, variant);
}

} // export namespace Common