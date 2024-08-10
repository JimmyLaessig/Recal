cmake_minimum_required(VERSION 3.28)

project(Log20)

###############################################################################
# Enable C++ 20 and modules support
###############################################################################

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS FALSE)
set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_VISIBILITY_INLINES_HIDDEN TRUE)

option(LOG20_BUILD_TESTS "Enable/Disable building Log20 unit tests" ON)

add_subdirectory(src/Log20)

if(LOG20_BUILD_TESTS)
	find_package(Catch2 REQUIRED)
	add_subdirectory(Test)
endif(LOG20_BUILD_TESTS)