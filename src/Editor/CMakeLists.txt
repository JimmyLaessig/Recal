################################################################################
# This CMakeLists.txt contains the build configuration for the GameEDITOR 
# library. It contains the library definition and install configuration of the
################################################################################

# GNUInstallDirs contains a well-established directory structure definition for cpp libraries
#include(GNUInstallDirs)
#
## The name of the GameEDITOR library
#set(EDITOR_TARGET_NAME Editor)
#
## The source location of the GameEDITOR library
#set(EDITOR_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
#
## The source location of the public GameEDITOR headers
#set(EDITOR_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include/${EDITOR_TARGET_NAME})
#
## Compile the Selection Shader
#compileShader(Selection Shaders
#              ${EDITOR_SRC_DIR}/Rendering/Shaders/generated/SelectionShader.h
#              ${EDITOR_SRC_DIR}/Rendering/Shaders/FullscreenQuad.vertex.glsl
#              ${EDITOR_SRC_DIR}/Rendering/Shaders/Selection.fragment.glsl)
#
#set(EDITOR_RENDERING_SHADER_SOURCES
#    ${EDITOR_SRC_DIR}/Rendering/Shaders/FullscreenQuad.vertex.glsl
#    ${EDITOR_SRC_DIR}/Rendering/Shaders/Selection.fragment.glsl)
#
#set(EDITOR_RENDERING_SHADERS_GENERATED
#    ${EDITOR_SRC_DIR}/Rendering/Shaders/generated/SelectionShader.h)
#
## Definition of component public header files of the GameEDITOR library
## These headers will be copying alongside the library during install
#set(EDITOR_SOURCES
#    ${EDITOR_SRC_DIR}/EditorLayer.cpp
#    ${EDITOR_SRC_DIR}/EditorOverlayRenderer.cpp
#    ${EDITOR_SRC_DIR}/EditorApplication.cpp
#)
#
## Definition of component public header files of the GameEDITOR library
## These headers will be copying alongside the library during install
#set(EDITOR_PUBLIC_INCLUDES
#    ${EDITOR_INCLUDE_DIR}/EditorLayer.ixx
#    ${EDITOR_INCLUDE_DIR}/EditorOverlayRenderer.ixx
#    ${EDITOR_INCLUDE_DIR}/EditorApplication.ixx
#)
#
#################################################################################
## Organize files in the IDE
#################################################################################
#
#source_group("include/Editor/" FILES ${EDITOR_PUBLIC_INCLUDES})
#
#source_group("src/" FILES ${EDITOR_SOURCES})
#
#source_group("src/Rendering/" FILES ${EDITOR_RENDERING_SOURCES})
#source_group("src/Rendering/Shaders" FILES ${EDITOR_RENDERING_SHADER_SOURCES})
#source_group("src/Rendering/Shaders/generated" FILES ${EDITOR_RENDERING_SHADERS_GENERATED})
#
#################################################################################
## Definition of all public header files of the GameEDITOR library
## These headers will be copying alongside the library during install
#################################################################################
#
#if (MSVC)
#    option(BUILD_SHARED_LIBRARY "Build and link ${EDITOR_TARGET_NAME} as a shared library" ON)
#else()
#    option(BUILD_SHARED_LIBRARY "Build and link ${EDITOR_TARGET_NAME} as a shared library" OFF)
#endif()
#
#################################################################################
## Create the GameEDITOR library
#################################################################################
#
#set(EDITOR_SOURCES
#    ${EDITOR_SOURCES}
#    ${EDITOR_RENDERING_SOURCES}
#    ${EDITOR_RENDERING_SHADER_SOURCES}
#    ${EDITOR_RENDERING_SHADERS_GENERATED}
#)
#
#set(EDITOR_PUBLIC_INCLUDES
#    ${EDITOR_PUBLIC_INCLUDES})
#
## All files listed here are included into the
## library. Header files do not need the be included here, but if included, they
## are visible in code editors (i.e. Visual Studio).
## (https://cmake.org/cmake/help/latest/command/add_library.ixxtml)
#add_library(${EDITOR_TARGET_NAME}
#    		${EDITOR_SOURCES}
#            ${EDITOR_PUBLIC_INCLUDES})
#
#################################################################################
## Generate the export header
#################################################################################
#
## The GenerateExportHeader module contains the function definition for generate_export_header()
#include(GenerateExportHeader)
#
## The generate_export_header function is used to generate a file containing
## suitable preprocessor for the EXPORT macros to be used in library classes. The
## header is automatically populated with the proper preprocessor defines based
## on the platform and build type (shared, static). 
## (https://cmake.org/cmake/help/latest/module/GenerateExportHeader.ixxtml)
#generate_export_header(${EDITOR_TARGET_NAME}
#    EXPORT_FILE_NAME
#        ${EDITOR_INCLUDE_DIR}/System.ixx
#    EXPORT_MACRO_NAME 
#        EDITOR_API)
#
##target_compile_definitions(${EDITOR_TARGET_NAME}
##    PUBLIC "$<$<NOT:$<BOOL:${BUILD_SHARED_LIBRARY}>>:EDITOR_STATIC_DEFINE>")
#
#################################################################################
## Set target properties
#################################################################################
#
## The command target_include_directories specifies the directories used when 
## compiling a given target.
## To provide a consistent include paths in source files, regardless of the target
## being build or installed. The BUILD_INTERFACE and INSTALL_INTERFACE generator 
## expressions are used to distinguish between the build tree and the install tree.
## (https://cmake.org/cmake/help/latest/command/target_include_directories.ixxtml)
#target_include_directories(${EDITOR_TARGET_NAME}
#	PUBLIC
#		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
#		$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
#    PRIVATE
#        stb::stb
#        imgui::stb::stb
#)
#
## The visual studio compiler creates a .pdb files containing the debug 
## information of the library. Setting the following property ensures the
## correct naming and output directory of the .pdb file.
#if(MSVC)
#    set_target_properties(${EDITOR_TARGET_NAME} PROPERTIES
#        COMPILE_PDB_NAME ${EDITOR_TARGET_NAME}
#        COMPILE_PDB_OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR})
#endif()
#
#################################################################################
## Link the GameEDITOR library with the following libraries
#################################################################################
## (https://cmake.org/cmake/help/latest/command/target_link_libraries.ixxtml)
#
#target_link_libraries(${EDITOR_TARGET_NAME}
#    PUBLIC
#    Engine
#)


###############################################################################
# Install step the GameEDITOR library
###############################################################################

# Install the GameEDITOR library target
#install(TARGETS ${EDITOR_TARGET_NAME}
#        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
#        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
#        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
#)

# Install the public headers
#install(FILES ${EDITOR_PUBLIC_INCLUDES}
#              ${EDITOR_INCLUDE_DIR}/System.ixx
#        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${EDITOR_TARGET_NAME}
#)

# Install the .pdb file in debug builds (MSVC only)
#if(MSVC)
#    install(FILES "${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>/${EDITOR_TARGET_NAME}.pdb"
#            DESTINATION ${CMAKE_INSTALL_LIBDIR}
#            CONFIGURATIONS Debug RelWithDebInfo
#            OPTIONAL
#            )
#endif()