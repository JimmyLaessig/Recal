//module;
//
//#include <Common/System.hpp>
//
//#include <memory>
//
//export module Multiton;
//
//import Common.NonCopyable;
//
//export namespace Common
//{
//
///// Base class for the multiton programming pattern
//template<typename T>
//class COMMON_API Multiton : Common::NonCopyable
//{
//public:
//
//	/// Get the current instance
//	static T& instance();
//
//	/// Set the current instance
//	static void setInstance(std::unique_ptr<T>&& instance);
//
//	/// Set the current instance
//	template<typename U>
//	static void setInstance();
//
//private:
//
//	static std::unique_ptr<T> sInstance;
//
//};
//
//} // export namespace Common
//
//template<typename T>
//std::unique_ptr<T> Common::Multiton<T>::sInstance = nullptr;
//
//template<typename T>
//T&
//Common::Multiton<T>::instance()
//{
//	return *sInstance;
//}
//
//
//template<typename T>
//void
//Common::Multiton<T>::setInstance(std::unique_ptr<T>&& instance)
//{
//	sInstance = instance;
//}
//
//
//template<typename T>
//template<typename U>
//void
//Common::Multiton<T>::setInstance()
//{
//	setInstance(std::make_unique<U>());
//}