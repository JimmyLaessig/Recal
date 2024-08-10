# The name of the ShaderCompiler example app executable
set(SHADER_COMPILER_TARGET_NAME ShaderCompiler)

# The source location of the ShaderCompiler example app executable
set(SHADER_COMPILER_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)


set(SHADER_COMPILER_SOURCES
    ${SHADER_COMPILER_SRC_DIR}/main.cpp
    ${SHADER_COMPILER_SRC_DIR}/GLSLShaderCompiler.cpp
    ${SHADER_COMPILER_SRC_DIR}/GLSLShaderCompiler.h
    ${SHADER_COMPILER_SRC_DIR}/GLSLShaderIncluder.cpp
    ${SHADER_COMPILER_SRC_DIR}/GLSLShaderIncluder.h
    ${SHADER_COMPILER_SRC_DIR}/HeaderGenerator.cpp
    ${SHADER_COMPILER_SRC_DIR}/HeaderGenerator.h)

set(SHADER_COMPILER_CMAKE_FILE ${CMAKE_CURRENT_SOURCE_DIR}/cmake/ShaderCompiler.cmake.in)

###############################################################################
# Organize files in the IDE
###############################################################################

source_group("src/" FILES  ${SHADER_COMPILER_SOURCES})
source_group("cmake/" FILES ${SHADER_COMPILER_CMAKE_FILE})

# Add the example app executable target
add_executable(${SHADER_COMPILER_TARGET_NAME}
               ${SHADER_COMPILER_SOURCES}
               ${SHADER_COMPILER_CMAKE_FILE})

find_package(glfw3)
find_package(GLEW)
find_package(CLI11)
find_package(shaderc)

# Link the example app against the ShaderCompiler library
target_link_libraries(${SHADER_COMPILER_TARGET_NAME}
                      glfw
                      GLEW::GLEW
                      CLI11::CLI11
                      shaderc::shaderc
                      spirv-reflect-static)

set(SHADER_COMPILER_EXECUTABLE ${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>/${SHADER_COMPILER_TARGET_NAME}.exe)

configure_file(${SHADER_COMPILER_CMAKE_FILE} ${CMAKE_BINARY_DIR}/ShaderCompiler.cmake @ONLY)

install(TARGETS ${SHADER_COMPILER_TARGET_NAME}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR})
