# Type System

Determining the exact class of components or other polymorphics objects at runtime requires detailed type information. For that, C++ provides [Runtime Type Information (RTTI)](https://learn.microsoft.com/en-us/cpp/cpp/run-time-type-information?view=msvc-170). However, there are valid arguments against using RTTI in performance-critical applications. 

Reef engine utilizes it's own little type system that allows to query type information of an object at runtime using some C++ 20 and compiler-specific tricks.

## Requirements

### 1. Ease of use

The type system must be easy to use. Adding new components or classes must only require one additional code addition (on top of the usual minimum code to create a new class with inheritance). 


For example, Unreal Engine extensively uses macro definitions to add a new class or struct to their type system. A code preprocessor is required to parse the macro definitions and auto-generate the type information before compilation:

```C++
#pragma once
#include "MyCustomComponent.generated.h"

UCLASS()
class UMyCustomComponent: public UComponent
{
    GENERATE_BODY()
    ...
};
```

In total, there are three additional lines of code that are required to add a component to the Unreal type system. Surely, we can do better than that.

### 2. Don't use macros

The arguments why not to use macros are manyfold. I won't go into detail as there are far more qualified people that have argued about it on the internet. Ultimately, I turned to Bjarne Stroustrup's [guidelines on marco usage](https://www.stroustrup.com/bs_faq2.html#macro) to convince me.

The C++20 module system only supports macros in a very limited form. For example, the `#include` macro is only allowed in global module fragments. Most important, macros cannot be exported from modules and are therefore not visible to module consumers. 

### 3. 

Creating the 


## Implementation

Let's begin by implementing the backbone of the type system, the `Type` class. The ´Type´ class consists of a name and a unique id. In order to make the `Type` class constructible at compile time, it must not use heap allocations. 

### Fetching the class name at compile time

To have the class name available at runtime, we need a way to obtain it at compile time. To achieve that, we make use of a compiler-spcific preprocessor definition. For MSVC, we utilize the `__FUNCSIG__` definition, which contains the signature of the function from which it it called. We use the template function trick, which enables `__FUNCSIG__` to contain the class name. We do a simple string analysis to fetch to substring that contains the actual class name. Note that all of this is doable at compile time.

```C++

template<typename Class>
constexpr static std::string_view className()
{
#ifdef  _MSC_VER
    // MSVC predefines the '__FUNCSIG__' preprocessor definition, which, for the templated function, contains the name
    // of the template argument.
    constexpr std::string_view fullName{ __FUNCSIG__ };
    constexpr std::string_view left{ "Engine::TypeFactory::className<" };
    constexpr std::string_view right{ ">(void)" };

    constexpr auto leftIndex = fullName.find(left);
    static_assert(leftIndex != std::string_view::npos);
    constexpr auto rightIndex = fullName.find(right, leftIndex);
    static_assert(rightIndex != std::string_view::npos);

    auto start = leftIndex + left.size();
    auto length = rightIndex - start;

    return fullName.substr(start, length);
#else //  _MSC_VER
    static_assert(false && "Unsupported compiler");
#endif
}
```

### Calculating the type id at compile time

Fetching type information at runtime must be performant and we should optimize lookup as much as possible. This includes omitting expensive string comparison or hash calculations at runtime. `std::hash` operator is not a constant expression and different compilers implement different hashing algorithms. Therefore, we are left with no option but to implement string hashing our own. Luckily, there are many open-source implementations already available, so it's just a matter of choosing one. I went with one implementation available on [Github](https://gist.github.com/iwanders/8e1cb7b92af2ccf8d1a73450d771f483).

## 

## 