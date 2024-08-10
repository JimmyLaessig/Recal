//module;
//
//#include <Engine/System.h>
//
//#include <concepts>
//#include <type_traits>
//#include <memory>
//
//#include <concepts>
//#include <string>
//#include <string_view>
//
//#include <nlohmann/json.hpp>
//
//#include <iostream>
//
//export module Engine.Serializable;
//
//import Engine.IComponent;
//
//import Engine.Object;
//import Engine.Type;
//import Engine.TypeRegistry;
//import Engine.PerFrameCallbacks;
//
//export namespace Reef
//{
//
//template<size_t N>
//struct Literal
//{
//    constexpr Literal(const char(&str)[N])
//    {
//        std::copy_n(str, N, value);
//    }
//
//    char value[N];
//};
//
//
//template<typename T, typename ...Args>
//concept HasEmplace = requires (T t, Args... args)
//{
//    { t.emplace(args...) } -> std::convertible_to<T&>;
//};
//
//
//template<typename T, Literal Name>
//class Property
//{
//public:
//
//    Property()
//    {
//        registerWithSerializable();
//    }
//
//
//    Property(const T& value)
//        : mValue(value)
//    {
//        registerWithSerializable();
//    }
//
//
//    Property(T&& value)
//        : mValue(value)
//    {
//        registerWithSerializable();
//    }
//
//
//    T& value()
//    {
//        return mValue;
//    }
//
//
//    const T& value() const
//    {
//        return mValue;
//    }
//
//
//    template <typename ...Args>
//    T& emplace(Args... args)
//    {
//        if constexpr (HasEmplace<T, Args...>)
//        {
//            mValue.emplace(std::forward<Args...>(args...));
//        }
//        else
//        {
//            mValue = T(std::forward<Args...>(args...));
//
//        }
//        return mValue;
//    }
//
//
//    template<typename U>
//    requires std::assignable_from<T, U>
//    Property<T, Name>& operator=(const U& rhs)
//    {
//        mValue = rhs;
//        return *this;
//    }
//
//
//    template<typename U>
//    Property<T, Name>& operator=(U&& rhs)
//    {
//        mValue = rhs;
//        return *this;
//    }
//
//
//    T* operator->()
//    {
//        return &mValue;
//    }
//
//
//    const T* operator->() const
//    {
//        return &mValue;
//    }
//
//
//    operator T() const { return mValue; }
//
//
//    template<typename U>
//    requires std::equality_comparable<T, U>
//    bool operator==(const U& rhs)
//    {
//        return mValue == rhs;
//    }
//
//
//    template<typename U>
//    requires std::totally_ordered<T, U>
//    bool operator>(const U & rhs)
//    {
//        return mValue == rhs;
//    }
//
//
//    template<typename U>
//    requires std::totally_ordered<T, U>
//    bool operator >= (const U& rhs)
//    {
//        return mValue == rhs;
//    }
//
//private:
//
//    void registerWithSerializable();
//
//    /// The wrapped value of the property
//    T mValue;
//};
//
//
//class ENGINE_API Serializable
//{
//public:
//
//    Serializable()
//    {
//        sCurrent = this;
//    }
//
//
//    void registerProperty(std::string_view s)
//    {
//        std::cout << s << std::endl;
//    }
//
//    /*void addProperty(Property&& property)
//    {
//        mFields.push_back(property);
//    }*/
//
//    static inline thread_local Serializable* sCurrent{ nullptr };
//
//private:
//
//    //std::vector<Property> mFields;
//};
//
//} // export namespace Reef
//
//
//
//template<typename T, Engine::Literal Name>
//void
//Engine::Property<T, Name>::registerWithSerializable()
//{
//    if (auto owner = Engine::Serializable::sCurrent)
//    {
//        owner->registerProperty(Name.value);
//    }
//    else
//    {
//        // TODO: Log error
//    }
//}