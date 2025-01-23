################################################################################
# This CMakeLists.txt contains the build configuration for the Common 
# library. It contains the library definition and install configuration of the
################################################################################

# The name of the Common library
set(COMMON_TARGET_NAME Common)

# The source location of the RenderingEngine library
set(COMMON_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

# The source location of the public RenderingEngine headers
set(COMMON_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include/${COMMON_TARGET_NAME})

###############################################################################
# Target-specific definition of static or shared build
###############################################################################

option(COMMON_BUILD_SHARED_LIBRARY "Build and link ${COMMON_TARGET_NAME} as a shared library" OFF)

if(COMMON_BUILD_SHARED_LIBRARY)
    set(COMMON_TARGET_TYPE STATIC)
else()
    set(COMMON_TARGET_TYPE SHARED)
endif()

###############################################################################
# Create the Common library
###############################################################################

add_library(${COMMON_TARGET_NAME} ${COMMON_TARGET_TYPE})

# Definition of all public module files of the RenderingEngine library
# These headers will be copying alongside the library during install
set(COMMON_PUBLIC_INCLUDES
    ${COMMON_INCLUDE_DIR}/AsyncTaskQueue.hpp
    ${COMMON_INCLUDE_DIR}/CRC32.hpp
    ${COMMON_INCLUDE_DIR}/Event.hpp
    ${COMMON_INCLUDE_DIR}/Finally.hpp
    ${COMMON_INCLUDE_DIR}/Multiton.hpp
    ${COMMON_INCLUDE_DIR}/NonCopyable.hpp
    ${COMMON_INCLUDE_DIR}/Singleton.hpp
    ${COMMON_INCLUDE_DIR}/SmartPointer.hpp
    ${COMMON_INCLUDE_DIR}/TypeName.hpp
    ${COMMON_INCLUDE_DIR}/Visitor.hpp
)

set(COMMON_SOURCES
    ${COMMON_SRC_DIR}/AsyncTaskQueue.cpp
)

###############################################################################
# Configure the library
###############################################################################

coral_configure_target(${COMMON_TARGET_NAME} 
    PUBLIC_HEADERS ${COMMON_PUBLIC_INCLUDES} 
    PRIVATE_HEADERS ${COMMON_SOURCES})

###############################################################################
# Organize files in the IDE
###############################################################################

source_group("include/${COMMON_TARGET_NAME}" FILES ${COMMON_PUBLIC_INCLUDES})
source_group("src/${COMMON_TARGET_NAME}" FILES ${COMMON_SOURCES})