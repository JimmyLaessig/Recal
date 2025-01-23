###############################################################################
# The name of the Material Generator library
###############################################################################
set(MATERIAL_GENERATOR_TARGET_NAME MaterialGenerator)

# The source location of the MaterialGenerator library
set(MATERIAL_GENERATOR_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

set(MATERIAL_GENERATOR_PUBLIC_INCLUDES
    ${CMAKE_CURRENT_SOURCE_DIR}/include/MaterialGenerator/MaterialGenerator.h)

set(MATERIAL_GENERATOR_SOURCES
    ${MATERIAL_GENERATOR_SRC_DIR}/MaterialGenerator.cpp)

###############################################################################
# Organize files in the IDE
###############################################################################

source_group("include/MaterialGenerator/" FILES  ${MATERIAL_GENERATOR_PUBLIC_INCLUDES})
source_group("src/" FILES  ${MATERIAL_GENERATOR_SOURCES})
source_group("cmake/" FILES ${MATERIAL_GENERATOR_CMAKE_FILE})

###############################################################################
# Create the Material Generator  library
###############################################################################

add_library(${MATERIAL_GENERATOR_TARGET_NAME} STATIC)

coral_configure_target(${MATERIAL_GENERATOR_TARGET_NAME}
          PUBLIC_HEADERS
            ${MATERIAL_GENERATOR_PUBLIC_INCLUDES}
          SOURCES
            ${MATERIAL_GENERATOR_SOURCES}
        PRIVATE_DEPENDENCIES
            ShaderCompiler)
