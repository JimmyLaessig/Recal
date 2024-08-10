# What is Log20?

Log20 is a macroless and modularized logging interface that utilizes C++ 20 features. Log20 works as an example implementation of a module-based library and therefore might not be fully compatible with older compilers and language version.

The arguments why not to use macros are manyfold. I won't go into detail as there are far more qualified people that have argued about it on the internet. Ultimately, I turned to Bjarne Stroustrup's [guidelines on marco usage](https://www.stroustrup.com/bs_faq2.html#macro) to convince me.

The C++20 module system only supports macros in a very limited form. For example, the `#include` macro is only allowed in global module fragments. Most important, macros cannot be exported from modules and are therefore not visible to module consumers. However, you can continue to provide macros with header files that must beincluded in the global module fragment. Ultimately, C++20 is equipped with all feature to replace one of the most common macro use cases, e.g. logging, with pure C++ with the helps of variadic templates, `std::format` and `std::source_location`.

## Build

```bash
git clone https://github.com/JimmyLaessig/Log20
```
Build the logging library with the default logger: 

```
cd Log20
mkdir build
cd build
cmake configure ../ -DLOG20_BUILD_TESTS=ON
cmake --build . 
cmake --install . --prefix "<Path_to_install_location>"
```

Or add the Log20 library as subdirectory to your cmake project:

```cmake
# CMakeLists.txt
add_subdirectory(<PATH_TO_LOG20>)

...

target_link_libraries(MyTarget PUBLIC|PRIVATE Log20)
```

## Requirements

* CMake 3.27.4
* Visual Studio 2022 17.6.2, MSVC 1939 (Windows only)
* Conan (Optional to download Catch2)

## Platforms

* Windows (Visual Studio 2022 17.6.2, MSVC 1939)