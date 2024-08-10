

#set(ENGINE_TEST_TARGET_NAME Engine.Tests)


#set(TEST_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)


#set(TEST_SOURCES
#    ${TEST_SRC_DIR}/CatchMain.cpp
#)

#set(ENGINE_RENDERING_TESTS
#   )

#set(ENGINE_CORE_TESTS
#)

#add_executable(${ENGINE_TEST_TARGET_NAME} 
#    ${TEST_SOURCES}
#    ${ENGINE_RENDERING_TESTS}
#    ${ENGINE_CORE_TESTS})


#set(CATCH_BUILD_TESTING OFF CACHE BOOL "" FORCE)
#set(CATCH_INSTALL_DOCS OFF CACHE BOOL "" FORCE)
#set(CATCH_INSTALL_HELPERS OFF CACHE BOOL "" FORCE)

#set(CMAKE_CXX_STANDARD_REQUIRED ON)
#set_target_properties(${ENGINE_TEST_TARGET_NAME} PROPERTIES CXX_STANDARD 20)

#find_package(Catch2 REQUIRED)

#target_link_libraries(${ENGINE_TEST_TARGET_NAME} PRIVATE
#    CONAN_PKG::catch2
#)

#find_package(catch2 REQUIRED)

#include(CTest)

#include(Catch.cmake)
# automatically enable catch2 to generate ctest targets
#if(CONAN_CATCH2_ROOT_DEBUG)
#  include(${CONAN_CATCH2_ROOT_DEBUG}/lib/cmake/Catch2/Catch.cmake)
#else()
#  include(${CONAN_CATCH2_ROOT}/lib/cmake/Catch2/Catch.cmake)
#endif()



#catch_discover_tests(${ENGINE_TEST_TARGET_NAME})

# Depending whether or not the library was built as static or shared
# copy the DLL to the binary directory of the excutable so that it
# can be found at runtime. 
#if (BUILD_SHARED_LIBRARY)

#    get_target_property(CPPTEMPLATE_BINARY_DIR RenderingEngine BINARY_DIR)

#    set (CPPTEMPLATE_BINARY_FILE "${CPPTEMPLATE_BINARY_DIR}/$<CONFIG>/RenderingEngine.dll")

#    add_custom_command(TARGET ${TEST_TARGET_NAME} POST_BUILD
#        COMMAND ${CMAKE_COMMAND} -E copy_if_different
#        ${CPPTEMPLATE_BINARY_FILE}
#        ${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>
#    )
#endif()



