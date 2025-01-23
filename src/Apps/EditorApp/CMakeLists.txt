# The name of the Editor app executable
set(EDITOR_APP_TARGET_NAME EditorApp)

add_executable(${EDITOR_APP_TARGET_NAME})

# The source location of the RenderingEngine example app executable
set(EDITOR_APP_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

set(EDITOR_APP_SOURCES
    ${EDITOR_APP_SRC_DIR}/main.cpp
)

coral_configure_target(${EDITOR_APP_TARGET_NAME}
          SOURCES ${EDITOR_APP_SOURCES}
          PRIVATE_HEADERS ${EDITOR_APP_GENERATED_SHADER_HEADERS}
          PRIVATE_DEPENDENCIES 
              Engine
          )
