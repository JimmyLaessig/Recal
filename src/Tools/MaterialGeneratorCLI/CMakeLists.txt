###############################################################################
# The name of the Material Generator command-line executable
###############################################################################

set(MATERIAL_GENERATOR_CLI_TARGET_NAME MaterialGeneratorCLI)

set(MATERIAL_GENERATOR_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp)

#set(MATERIAL_GENERATOR_CMAKE_FILE ${CMAKE_CURRENT_SOURCE_DIR}/cmake/MaterialGenerator.cmake.in)

###############################################################################
# Organize files in the IDE
###############################################################################

source_group("src/" FILES  ${MATERIAL_GENERATOR_SOURCES})
source_group("cmake/" FILES ${MATERIAL_GENERATOR_CMAKE_FILE})

find_package(CLI11)

# Add the example app executable target
add_executable(${MATERIAL_GENERATOR_CLI_TARGET_NAME}
               ${MATERIAL_GENERATOR_SOURCES}
               ${MATERIAL_GENERATOR_CMAKE_FILE})

coral_configure_target(${MATERIAL_GENERATOR_CLI_TARGET_NAME}
    SOURCES ${MATERIAL_GENERATOR_SOURCES}
    PRIVATE_DEPENDENCIES
        MaterialGenerator
        CLI11::CLI11)

#set(MATERIAL_GENERATOR_EXECUTABLE ${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>/${MATERIAL_GENERATOR_CLI_TARGET_NAME}.exe)

#configure_file(${MATERIAL_GENERATOR_CMAKE_FILE} ${CMAKE_BINARY_DIR}/MaterialGenerator.cmake @ONLY)

install(TARGETS ${MATERIAL_GENERATOR_CLI_TARGET_NAME}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR})
