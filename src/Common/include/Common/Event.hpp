//module;
//
//#include <unordered_map>
//
//#include <functional>
//
//#include <Common/System.hpp>
//
//export module Common.Event;
//
//import Common.NonCopyable;
//
//export namespace Common
//{
//
//template<typename ...Args>
//class COMMON_API Event : public Common::NonCopyable
//{
//public:
//
//	template<typename Subscriber, typename Function>
//	void subscribe(Subscriber& subscriber, Function fun) const
//	{
//		mSubscribers[&subscriber] = fun;
//	}
//
//	template<typename Subscriber>
//	void unsubscribe(Subscriber& subscriber) const
//	{
//		mSubscribers.erase(&subscriber);
//	}
//
//	void broadcast(Args... args)
//	{
//		for (auto& [subscriber, fun] : mSubscribers)
//		{
//			fun(args...);
//		}
//	}
//
//private:
//
//	mutable std::unordered_map<void*, std::function<void(Args...)>> mSubscribers;
//
//};
//
//} // namespace Reef