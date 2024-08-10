
# The name of the StandaloneApp example app executable
set(CORAL_DEMO_APP_TARGET_NAME CoralDemoApp)

# The source location of the RenderingEngine example app executable
set(CORAL_DEMO_APP_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

set(CORAL_DEMO_APP_SHADER_DIR ${CORAL_DEMO_APP_SRC_DIR}/Shaders)

set(CORAL_DEMO_APP_INCLUDE_DIR ${CMAKE_CURRENT_INCLUDE_DIR}/include)

set(CORAL_DEMO_APP_GENERATED_INCLUDE_DIR ${CMAKE_CURRENT_BINARY_DIR}/${CORAL_DEMO_APP_TARGET_NAME}/generated/include)

set(CORAL_DEMO_APP_SOURCES
    ${CORAL_DEMO_APP_SRC_DIR}/main.cpp
)


###############################################################################
# Set shader files
###############################################################################

set(CORAL_DEMO_APP_SHADER_SOURCES
    ${CORAL_DEMO_APP_SHADER_DIR}/UniformColor.vert.glsl
    ${CORAL_DEMO_APP_SHADER_DIR}/UniformColor.frag.glsl)

compileShader(UniformColor Shaders
              ${CORAL_DEMO_APP_GENERATED_INCLUDE_DIR}/Shaders/UniformColor.h
              ${CORAL_DEMO_APP_SHADER_DIR}/UniformColor.vert.glsl
              ${CORAL_DEMO_APP_SHADER_DIR}/UniformColor.frag.glsl
              ${CORAL_DEMO_APP_SHADER_DIR})

set(CORAL_DEMO_APP_GENERATED_SHADER_HEADERS ${CORAL_DEMO_APP_GENERATED_INCLUDE_DIR}/Shaders/UniformColor.h)

###############################################################################
# Organize files in the IDE
###############################################################################

find_package(glm)
find_package(glfw3)
find_package(stb)

AddTarget(TARGET_NAME ${CORAL_DEMO_APP_TARGET_NAME}
          TARGET_TYPE EXECUTABLE
          MODULE_SOURCES ${CORAL_DEMO_APP_SOURCES}
          PRIVATE_HEADERS ${CORAL_DEMO_APP_GENERATED_SHADER_HEADERS}
          PRIVATE_DEPENDENCIES 
              Coral
              glfw::glfw
              glm::glm
              stb::stb
          )

target_sources(${CORAL_DEMO_APP_TARGET_NAME}
    PRIVATE ${CORAL_DEMO_APP_SHADER_SOURCES}
)

source_group("include/generated/Shaders" FILES ${CORAL_DEMO_APP_GENERATED_SHADER_HEADERS})
source_group("src" FILES ${CORAL_DEMO_APP_SOURCES})
source_group("Shaders" FILES ${CORAL_DEMO_APP_SHADER_SOURCES})
