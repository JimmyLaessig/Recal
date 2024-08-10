function(find_include_directories headers result)
    set(includes "")
    foreach(file IN LISTS ${headers})
        string(FIND ${file} "include" found)

        if (found GREATER_EQUAL 0)
            string(SUBSTRING ${file} 0 ${found} INCLUDE_DIRECTORY)
    
            list(APPEND includes ${INCLUDE_DIRECTORY}include)
        endif()
        
    endforeach()

    list(REMOVE_DUPLICATES includes)

    set(${result} ${includes} PARENT_SCOPE)

endfunction()


function(find_module_directories modules result)
    set(directories "")
    foreach(file IN LISTS ${modules})
        cmake_path(GET file PARENT_PATH directory)
        list(APPEND directories ${directory})
    endforeach()

    list(REMOVE_DUPLICATES directories)

    set(${result} ${directories} PARENT_SCOPE)
endfunction()


macro(AddTarget)

set(options BUILD_SHARED_LIBRARY)
set(oneValueArgs TARGET_NAME TARGET_TYPE)
set(multiValueArgs PUBLIC_MODULES PRIVATE_MODULES PUBLIC_HEADERS PRIVATE_HEADERS MODULE_SOURCES PUBLIC_DEPENDENCIES PRIVATE_DEPENDENCIES)

cmake_parse_arguments("INPUT" "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN} )

set(TARGET_NAME ${INPUT_TARGET_NAME})
set(TARGET_TYPE ${INPUT_TARGET_TYPE})
set(PUBLIC_MODULES ${INPUT_PUBLIC_MODULES})
set(PRIVATE_MODULES ${INPUT_PRIVATE_MODULES})
set(MODULE_SOURCES ${INPUT_MODULE_SOURCES})
set(PRIVATE_HEADERS ${INPUT_PRIVATE_HEADERS})
set(PUBLIC_HEADERS ${INPUT_PUBLIC_HEADERS})

set(PUBLIC_DEPENDENCIES ${INPUT_PUBLIC_DEPENDENCIES})
set(PRIVATE_DEPENDENCIES ${INPUT_PRIVATE_DEPENDENCIES})

if (TARGET_TYPE STREQUAL "STATIC_LIBRARY" OR TARGET_TYPE STREQUAL "SHARED_LIBRARY")
    set(BUILD_LIBRARY TRUE)
else()
    set(BUILD_LIBRARY FALSE)
endif()

###############################################################################
# Create the target
###############################################################################

if (TARGET_TYPE STREQUAL "STATIC_LIBRARY")
    add_library(${TARGET_NAME} STATIC)
elseif(TARGET_TYPE STREQUAL "SHARED_LIBRARY")
    add_library(${TARGET_NAME} SHARED)
elseif(TARGET_TYPE STREQUAL "EXECUTABLE")
    add_executable(${TARGET_NAME})
else()
    message(FATAL_ERROR "Invalid target type '${TARGET_TYPE}'")
endif()

###############################################################################
# Generate the export header
###############################################################################

if (BUILD_LIBRARY)

    # The GenerateExportHeader module contains the function definition for 
    # generate_export_header()
    include(GenerateExportHeader)

    string(TOUPPER "${TARGET_NAME}" TARGET_NAME_UPPERCASE)

    set(GENERATED_HEADER_DIR ${CMAKE_CURRENT_BINARY_DIR}/generated/include)
    set(EXPORT_HEADER ${GENERATED_HEADER_DIR}/${TARGET_NAME}/System.h)

    # The generate_export_header function is used to generate a file containing
    # suitable preprocessor for the EXPORT macros to be used in library classes.
    # The header is automatically populated with the proper preprocessor defines
    # based on the platform and build type (shared, static). 
    # (https://cmake.org/cmake/help/latest/module/GenerateExportHeader.html)
    generate_export_header(${TARGET_NAME}
        EXPORT_FILE_NAME
            ${EXPORT_HEADER}
        EXPORT_MACRO_NAME
            ${TARGET_NAME_UPPERCASE}_API)

    target_compile_definitions(${TARGET_NAME}
        PUBLIC "$<$<NOT:$<BOOL:${SHARED_LIBRARY}>>:${TARGET_NAME_UPPERCASE}_STATIC_DEFINE>")

    source_group("generated/include/${TARGET_NAME}" FILES ${EXPORT_HEADER})

    target_sources(${TARGET_NAME}
        PUBLIC
            FILE_SET public_export_header
                TYPE HEADERS
                BASE_DIRS ${CMAKE_CURRENT_BINARY_DIR}
                FILES ${EXPORT_HEADER})
    
    target_include_directories(${TARGET_NAME}
        PUBLIC $<BUILD_INTERFACE:${GENERATED_HEADER_DIR}>)

endif()

###############################################################################
# Add the source files to the target
###############################################################################

if (PUBLIC_HEADERS)
    target_sources(${TARGET_NAME}
        PUBLIC ${PUBLIC_HEADERS})
endif()

if (PRIVATE_HEADERS)
    target_sources(${TARGET_NAME}
        PRIVATE ${PRIVATE_HEADERS})
endif()

if (PUBLIC_MODULES)
    target_sources(${TARGET_NAME}
        PUBLIC
            FILE_SET public_modules
                TYPE CXX_MODULES
                FILES ${PUBLIC_MODULES})
endif()

if (PRIVATE_MODULES)
    target_sources(${TARGET_NAME}
        PRIVATE
            FILE_SET private_modules
                TYPE CXX_MODULES
                FILES ${PRIVATE_MODULES})
endif()

if (MODULE_SOURCES)
    target_sources(${TARGET_NAME}
        PRIVATE
            FILE_SET module_sources
                TYPE CXX_MODULES
                FILES ${MODULE_SOURCES})
endif()

###############################################################################
# Set target properties
###############################################################################

# CMake requires the language standard to be specified as compile feature
# when a target provides C++20 modules and the target will be installed 
target_compile_features(${TARGET_NAME} PUBLIC cxx_std_20)

set_target_properties(${TARGET_NAME} PROPERTIES LINKER_LANGUAGE CXX)

find_include_directories(PUBLIC_HEADERS PUBLIC_INCLUDE_DIRECTORIES)

find_include_directories(PRIVATE_HEADERS PRIVATE_INCLUDE_DIRECTORIES)

find_module_directories(PUBLIC_MODULES PUBLIC_MODULE_DIRECTORIES)

find_module_directories(PRIVATE_MODULES PRIVATE_MODULE_DIRECTORIES)

# The command target_include_directories specifies the directories used when 
# compiling a given target.
# To provide a consistent include paths in source files, regardless of the target
# being build or installed. The BUILD_INTERFACE and INSTALL_INTERFACE generator 
# expressions are used to distinguish between the build tree and the install tree.
# (https://cmake.org/cmake/help/latest/command/target_include_directories.html)

foreach (DIRECTORY IN LISTS PUBLIC_INCLUDE_DIRECTORIES PUBLIC_MODULE_DIRECTORIES)
    target_include_directories(${TARGET_NAME}
        PUBLIC $<BUILD_INTERFACE:${DIRECTORY}>)
endforeach()

foreach (DIRECTORY IN LISTS PRIVATE_INCLUDE_DIRECTORIES PRIVATE_MODULE_DIRECTORIES)
    target_include_directories(${TARGET_NAME}
        PRIVATE $<BUILD_INTERFACE:${DIRECTORY}>)
endforeach()

# The visual studio compiler creates a .pdb files containing the debug 
# information of the library. Setting the following property ensures the
# correct naming and output directory of the .pdb file.
if(MSVC)
    set_target_properties(${TARGET_NAME} PROPERTIES
        COMPILE_PDB_NAME ${TARGET_NAME}
        COMPILE_PDB_OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR})
endif()

###############################################################################
# Link the target
###############################################################################

# (https://cmake.org/cmake/help/latest/command/target_link_libraries.html)
target_link_libraries(${TARGET_NAME}
    PRIVATE
        ${PRIVATE_DEPENDENCIES}
    PUBLIC
        ${PUBLIC_DEPENDENCIES})

###############################################################################
# Install the target
###############################################################################

# GNUInstallDirs contains a well-established directory structure definition for
# cpp libraries
include(GNUInstallDirs)

# Install the .pdb file in debug builds (MSVC only)
if(MSVC)
    install(FILES "${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>/${TARGET_NAME}.pdb"
            DESTINATION ${CMAKE_INSTALL_LIBDIR}
            CONFIGURATIONS Debug RelWithDebInfo
            OPTIONAL)
endif()

add_dependencies(${TARGET_NAME} ShaderCompiler)

endmacro()