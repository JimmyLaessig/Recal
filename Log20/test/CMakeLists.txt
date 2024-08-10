###############################################################################
# This CMakeLists.txt contains the build and ctest configuration of the 
# Log20 library tests.
###############################################################################

set(LOG20_TEST_TARGET_NAME Log20.Tests)

###############################################################################
# Create the Log20 test executable
###############################################################################

add_executable(${LOG20_TEST_TARGET_NAME}
    ${LOG20_TEST_SOURCES})

# The source location of the Log20 tests
set(LOG20_TEST_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

# Definition of all source files of the Log20 tests
set(LOG20_TEST_SOURCES
    ${LOG20_TEST_SRC_DIR}/UnitTests.cpp
)

# Add the test source files
target_sources(${LOG20_TEST_TARGET_NAME} 
    PUBLIC ${LOG20_TEST_SOURCES})

###############################################################################
# Link the test executable
###############################################################################

target_link_libraries(${LOG20_TEST_TARGET_NAME}
    PRIVATE
        Log20
        Catch2::Catch2WithMain
)

# Depending whether or not the library was built as static or shared, copy the
# DLL to the binary directory of the excutable so that it can be found at 
# runtime.
if (LOG20_BUILD_SHARED_LIBRARY)
    get_target_property(LOG20_BINARY_DIR Log20 BINARY_DIR)
    if (MSVC)
        set(LOG20_BINARY_FILES "${LOG20_BINARY_DIR}/$<CONFIG>/")
    else()
        set(LOG20_BINARY_FILES "${LOG20_BINARY_DIR}/$<CONFIG>/")
        endif()
    
    add_custom_command(TARGET ${LOG20_TEST_TARGET_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${LOG20_BINARY_FILES} ${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>)
endif()

###############################################################################
# Setup CTest
###############################################################################

include(Catch)
include(CTest)

catch_discover_tests(${LOG20_TEST_TARGET_NAME} WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>)
