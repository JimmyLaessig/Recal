###############################################################################
# This CMakeLists.txt contains the build and install configuration of the Log20
# library.
###############################################################################

set(LOG20_TARGET_NAME Log20)

###############################################################################
# Target-specific definition of static or shared build
###############################################################################

option(LOG20_BUILD_SHARED_LIBRARY "Build and link ${LOG20_TARGET_NAME} as a shared library" OFF)

if (LOG20_BUILD_SHARED_LIBRARY)
    set(LOG20_LIBRARY_TYPE SHARED)
else()
    set(LOG20_LIBRARY_TYPE STATIC)
endif(LOG20_BUILD_SHARED_LIBRARY)

###############################################################################
# Create the Log20 library
###############################################################################

add_library(${LOG20_TARGET_NAME} ${LOG20_LIBRARY_TYPE})

# The source location of the Log20 library
set(LOG20_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

# The source location of the public Log20 headers
set(LOG20_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include/${LOG20_TARGET_NAME})

# Definition of all source files of the Log20 library
set(LOG20_SOURCES
    ${LOG20_SRC_DIR}/Logger.cxx
    ${LOG20_SRC_DIR}/AppName.cxx
    ${LOG20_SRC_DIR}/AsyncTaskQueue.cxx
    ${LOG20_SRC_DIR}/DefaultLogger.cxx
)

# Definition of all source files of the Log20 library
set(LOG20_PRIVATE_MODULES
    ${LOG20_SRC_DIR}/AppName.ixx
    ${LOG20_SRC_DIR}/AsyncTaskQueue.ixx
)

# Definition of all public module files of the Log20 library
# These headers will be copying alongside the library during install
set(LOG20_PUBLIC_MODULES
    ${LOG20_INCLUDE_DIR}/Log20.ixx
    ${LOG20_INCLUDE_DIR}/Logger.ixx
    ${LOG20_INCLUDE_DIR}/DefaultLogger.ixx
)

# Add the public-facing module definitions
target_sources(${LOG20_TARGET_NAME}
    PUBLIC
    FILE_SET public_modules 
    TYPE CXX_MODULES 
    FILES ${LOG20_PUBLIC_MODULES}
)

# Add the private module definitions
target_sources(${LOG20_TARGET_NAME}
    PRIVATE
    FILE_SET private_modules 
    TYPE CXX_MODULES 
    FILES ${LOG20_PRIVATE_MODULES}
)

# Add the module implementation files
target_sources(${LOG20_TARGET_NAME}
    PRIVATE
    FILE_SET module_implementations 
    TYPE CXX_MODULES 
    FILES ${LOG20_SOURCES}
)

################################################################################
# Generate the export header
################################################################################

set(LOG20_EXPORT_HEADER ${CMAKE_CURRENT_BINARY_DIR}/${LOG20_TARGET_NAME}/System.h)

include(GenerateExportHeader)

# The generate_export_header function is used to generate a file containing
# suitable preprocessor for the EXPORT macros to be used in library classes. The
# header is automatically populated with the proper preprocessor defines based
# on the platform and build type (shared, static). 
# (https://cmake.org/cmake/help/latest/module/GenerateExportHeader.html)
generate_export_header(${LOG20_TARGET_NAME}
    EXPORT_FILE_NAME
       ${LOG20_EXPORT_HEADER}
    EXPORT_MACRO_NAME 
        LOG20_API)

 # Add the module implementation files
target_sources(${LOG20_TARGET_NAME}
    PUBLIC
    FILE_SET public_headers 
    TYPE HEADERS
    BASE_DIRS ${CMAKE_CURRENT_BINARY_DIR}
    FILES ${LOG20_EXPORT_HEADER}
)

# The generated export header relies on this compile definition to distinguish 
# between static or shared build      
target_compile_definitions(${LOG20_TARGET_NAME} 
                           PUBLIC 
                           "$<$<NOT:$<BOOL:${LOG20_BUILD_SHARED_LIBRARY}>>:LOG20_STATIC_DEFINE>")

###############################################################################
# Set target properties
###############################################################################

# The command target_include_directories specifies the directories used when 
# compiling a given target.
# To provide a consistent include paths in source files, regardless of the
# target being build or installed. The BUILD_INTERFACE and INSTALL_INTERFACE 
# generator expressions are used to distinguish between the build tree and the 
# install tree.
# (https://cmake.org/cmake/help/latest/command/target_include_directories.html)
target_include_directories(${LOG20_TARGET_NAME}
	PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
		$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
    PRIVATE
        ${CMAKE_CURRENT_BINARY_DIR}
)

# The Visual Studio compiler creates a .pdb files containing the debug 
# information of the library. Setting the following property ensures the 
# correct naming and output directory of the .pdb file.
if(MSVC)
    set_target_properties(${LOG20_TARGET_NAME} PROPERTIES
        COMPILE_PDB_NAME ${LOG20_TARGET_NAME}
        COMPILE_PDB_OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR})
endif()

###############################################################################
# Install step the Log20 library
###############################################################################

# GNUInstallDirs contains a well-established directory structure definition for
# cpp libraries
include(GNUInstallDirs)

# Install the Log20 library target
install(TARGETS ${LOG20_TARGET_NAME}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR})

# Install the public module files
install(FILES ${LOG20_PUBLIC_MODULES}
              ${LOG20_EXPORT_HEADER}
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${LOG20_TARGET_NAME})

# Install the .pdb file in debug builds (MSVC only)
if(MSVC)
    install(FILES "${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>/${LOG20_TARGET_NAME}.pdb"
            DESTINATION ${CMAKE_INSTALL_LIBDIR}
            CONFIGURATIONS Debug RelWithDebInfo
            OPTIONAL)
endif()