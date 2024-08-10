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

# Definition of all public module files of the RenderingEngine library
# These headers will be copying alongside the library during install
set(COMMON_PUBLIC_MODULES
    ${COMMON_INCLUDE_DIR}/AsyncTaskQueue.ixx
    ${COMMON_INCLUDE_DIR}/CRC32.ixx
    ${COMMON_INCLUDE_DIR}/Event.ixx
    ${COMMON_INCLUDE_DIR}/Finally.ixx
    ${COMMON_INCLUDE_DIR}/Multiton.ixx
    ${COMMON_INCLUDE_DIR}/NonCopyable.ixx
    ${COMMON_INCLUDE_DIR}/Singleton.ixx
    ${COMMON_INCLUDE_DIR}/SmartPointer.ixx
    ${COMMON_INCLUDE_DIR}/TypeName.ixx
    ${COMMON_INCLUDE_DIR}/Visitor.ixx
)

set(COMMON_SOURCES
    ${COMMON_SRC_DIR}/AsyncTaskQueue.cxx
)

###############################################################################
# Target-specific definition of static or shared build
###############################################################################

option(COMMON_BUILD_SHARED_LIBRARY "Build and link ${COMMON_TARGET_NAME} as a shared library" OFF)

if(COMMON_BUILD_SHARED_LIBRARY)
    set(COMMON_TARGET_TYPE SHARED_LIBRARY)
else()
    set(COMMON_TARGET_TYPE STATIC_LIBRARY)
endif()

###############################################################################
# Create the Common library
###############################################################################

AddTarget(TARGET_NAME ${COMMON_TARGET_NAME} 
          TARGET_TYPE ${COMMON_TARGET_TYPE}
          PUBLIC_MODULES ${COMMON_PUBLIC_MODULES}
          MODULE_SOURCES ${COMMON_SOURCES})

###############################################################################
# Organize files in the IDE
###############################################################################

source_group("include/public/${COMMON_TARGET_NAME}" FILES ${COMMON_PUBLIC_MODULES})
source_group("src/${COMMON_TARGET_NAME}" FILES ${COMMON_SOURCES})