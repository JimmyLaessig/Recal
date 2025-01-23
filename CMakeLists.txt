cmake_minimum_required(VERSION 3.28)

# The global name of the project
set(PROJECT_NAME Reef)

# The global version of the Reef Engine project
set(REEF_ENGINE_VERSION_MAJOR 0)
set(REEF_ENGINE_VERSION_MINOR 1)
set(REEF_ENGINE_VERSION_PATCH 0)
set(REEF_ENGINE_PROJECT_VERSION ${REEF_ENGINE_VERSION_MAJOR}.${REEF_ENGINE_VERSION_MINOR}.${REEF_ENGINE_VERSION_PATCH})

project(${PROJECT_NAME} VERSION ${REEF_ENGINE_PROJECT_VERSION})

###############################################################################
# Enable C++ 20 and modules support
###############################################################################

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS FALSE)
set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_VISIBILITY_INLINES_HIDDEN TRUE)

include(cmake/ConfigureTarget.cmake)

add_subdirectory(ThirdParty)

add_subdirectory(src)

add_subdirectory(test)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
