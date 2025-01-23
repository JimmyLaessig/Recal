# Download conan.cmake automatically
if(NOT EXISTS "${CMAKE_CURRENT_BINARY_DIR}/conan.cmake")
  message(STATUS "Downloading conan.cmake from https://github.com/conan-io/cmake-conan")
  file(DOWNLOAD "https://raw.githubusercontent.com/conan-io/cmake-conan/develop/conan.cmake" "${CMAKE_CURRENT_BINARY_DIR}/conan.cmake")
endif()

include(${CMAKE_CURRENT_BINARY_DIR}/conan.cmake)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_BINARY_DIR} PARENT_SCOPE)
set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} ${CMAKE_CURRENT_BINARY_DIR} PARENT_SCOPE)

conan_cmake_configure(
    REQUIRES
        catch2/3.4.0
        glew/2.2.0
        glfw/3.3.8
        glm/cci.20220420
        imgui/cci.20230105+1.89.2.docking
        shaderc/2023.6
        cli11/2.4.2
        tinygltf/2.8.13
        vulkan-headers/1.3.268.0
        volk/1.3.268.0
        #vulkan-validationlayers/1.3.239.0
        vulkan-memory-allocator/cci.20231120
        vk-bootstrap/0.7

        nlohmann_json/3.11.2
  GENERATORS
        cmake_find_package_multi)

foreach(TYPE ${CMAKE_CONFIGURATION_TYPES})
    conan_cmake_autodetect(settings BUILD_TYPE ${TYPE})
    conan_cmake_install(PATH_OR_REFERENCE .
                        BUILD missing
                        REMOTE conancenter
                        SETTINGS ${settings})
endforeach()
