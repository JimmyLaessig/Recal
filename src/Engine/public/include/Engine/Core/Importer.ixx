//#ifndef ENGINE_SERIALIZER_H_
//#define ENGINE_SERIALIZER_H_
//
////#include <Engine/System.h>
//
////#include <string>
////#include <string_view>
////#include <sstream>
////#include <unordered_map>
////memory>;
////#include <functional>
//
//#include <yaml-cpp/yaml.h>
//
//namespace Reef
//{
//
//template<typename T>
//bool
//fromYAML(const YAML::Node& node, T& t)
//{
//	try
//	{
//		t = node.as<T>();
//
//		return true;
//	}
//	catch (const YAML::TypedBadConversion<T>&)
//	{
//		return false;
//	}
//}
//
//
//class Serializer
//{
//public:
//
//};
//
//} // namespace Reef
//
//#endif // UTIL_TYPE_H_