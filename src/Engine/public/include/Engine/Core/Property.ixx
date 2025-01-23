//#ifndef ENGINE_PROPERTY_H_
//#define ENGINE_PROPERTY_H_
//
////#include <Engine/System.hpp>
//
//#include <string>
//#include <string_view>
//#include <sstream>
//#include <unordered_map>
//memory>;
//#include <functional>
//
//
//namespace Reef
//{
//
//
//#define PROPERTY(TYPE, NAME, VALUE)							\
//public:														\
//	const TYPE& get##NAME() const { return m##NAME##; }		\
//															\
//	void set##NAME(const TYPE& value) { m##NAME = value; }	\
//															\
//	void set##NAME(TYPE&& value) { m##NAME = value; }		\
//private:													\
//															\
//	TYPE m##NAME{ VALUE };									\
//
//#define READ_ONLY_PROPERTY(TYPE, NAME, VALUE)				\
//public:														\
//	const TYPE& get##NAME() const { return m##NAME##; }		\
//															\
//private:													\
//															\
//	TYPE m##NAME{ VALUE };									\
//
//} // namespace Base
//
//#endif // !UTIL_TYPE_H_