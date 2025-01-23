###############################################################################
# The name of the Coral shader compiler command-line executable
###############################################################################
set(CORAL_SHADER_COMPILER_CLI_TARGET_NAME CoralShaderCompilerCLI)

set(CORAL_SHADER_COMPILER_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp)

set(CORAL_SHADER_COMPILER_CMAKE_FILE ${CMAKE_CURRENT_SOURCE_DIR}/cmake/CoralShaderCompiler.cmake.in)

###############################################################################
# Organize files in the IDE
###############################################################################

source_group("src/" FILES  ${CORAL_SHADER_COMPILER_SOURCES})
source_group("cmake/" FILES ${CORAL_SHADER_COMPILER_CMAKE_FILE})

find_package(CLI11)

###############################################################################
# add the Coral shader compiler executable
###############################################################################
add_executable(${CORAL_SHADER_COMPILER_CLI_TARGET_NAME}
               ${CORAL_SHADER_COMPILER_SOURCES}
               ${CORAL_SHADER_COMPILER_CMAKE_FILE})

coral_configure_target(${CORAL_SHADER_COMPILER_CLI_TARGET_NAME}
    SOURCES ${CORAL_SHADER_COMPILER_SOURCES}
    PRIVATE_DEPENDENCIES
        CoralShaderCompiler
        CLI11::CLI11)

###############################################################################
# Create the Cmake shader compiler command
###############################################################################

set(CORAL_SHADER_COMPILER_EXECUTABLE ${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>/${CORAL_SHADER_COMPILER_CLI_TARGET_NAME}.exe)

configure_file(${CORAL_SHADER_COMPILER_CMAKE_FILE} ${CMAKE_BINARY_DIR}/CoralShaderCompiler.cmake @ONLY)
