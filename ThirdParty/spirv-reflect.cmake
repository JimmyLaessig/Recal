include(FetchContent)

FetchContent_Declare(
  spirv-reflect
  GIT_REPOSITORY git@github.com:KhronosGroup/SPIRV-Reflect.git
  GIT_TAG vulkan-sdk-1.3.275.0
)

set(SPIRV_REFLECT_STATIC_LIB ON CACHE BOOL "" FORCE)
set(SPIRV_REFLECT_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(SPIRV_REFLECT_EXECUTABLE OFF CACHE BOOL "" FORCE)

FETCHContent_MakeAvailable(spirv-reflect)