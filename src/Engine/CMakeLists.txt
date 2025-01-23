################################################################################
# This CMakeLists.txt contains the build configuration for the Engine 
# library. It contains the library definition and install configuration of the
################################################################################

set(ENGINE_TARGET_NAME Engine)

###############################################################################
# Create the Engine library
###############################################################################

option(ENGINE_BUILD_SHARED_LIBRARY "Build and link ${ENGINE_TARGET_NAME} as a shared library" OFF)

if (ENGINE_BUILD_SHARED_LIBRARY)
    set(ENGINE_BUILD_TYPE SHARED)
else()
    set(ENGINE_BUILD_TYPE STATIC)
endif()

add_library(${ENGINE_TARGET_NAME} ${ENGINE_BUILD_TYPE})

# The source location of the public Engine headers
set(ENGINE_PUBLIC_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/public/include/${ENGINE_TARGET_NAME})
# The source location of the private Engine headers
set(ENGINE_PRIVATE_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/private/include/${ENGINE_TARGET_NAME})
# The source location of the Engine library
set(ENGINE_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/private/src)
# The shader location of the Engine library
set(ENGINE_RENDERING_SHADER_DIR ${ENGINE_SRC_DIR}/Rendering/Shaders)
# The location of the generated shader files
set(ENGINE_GENERATED_SHADER_DIR ${CMAKE_CURRENT_BINARY_DIR}/generated/private/include)

###############################################################################
# namespace Engine::Application 
###############################################################################

set(ENGINE_APPLICATION_PUBLIC_HEADERS
    ${ENGINE_PUBLIC_INCLUDE_DIR}/Application/Application.hpp
    ${ENGINE_PUBLIC_INCLUDE_DIR}/Application/CommandLineArguments.hpp
    ${ENGINE_PUBLIC_INCLUDE_DIR}/Application/EntryPoint.hpp
    ${ENGINE_PUBLIC_INCLUDE_DIR}/Application/Layer.hpp
    ${ENGINE_PUBLIC_INCLUDE_DIR}/Application/Window.hpp)

set(ENGINE_APPLICATION_PRIVATE_HEADERS
    ${ENGINE_PRIVATE_INCLUDE_DIR}/Application/ApplicationImpl.hpp
    ${ENGINE_PRIVATE_INCLUDE_DIR}/Application/WindowImpl.hpp)

set(ENGINE_APPLICATION_SOURCES
    ${ENGINE_SRC_DIR}/Application/Application.cpp
    ${ENGINE_SRC_DIR}/Application/ApplicationImpl.cpp
    ${ENGINE_SRC_DIR}/Application/CommandLineArguments.cpp
    ${ENGINE_SRC_DIR}/Application/Layer.cpp
    ${ENGINE_SRC_DIR}/Application/Window.cpp
    ${ENGINE_SRC_DIR}/Application/WindowImpl.cpp)

###############################################################################
# namespace Engine::Core
###############################################################################

set(ENGINE_CORE_PUBLIC_HEADERS
	${ENGINE_PUBLIC_INCLUDE_DIR}/Core/Material.hpp
    ${ENGINE_PUBLIC_INCLUDE_DIR}/Core/BoundingBox3D.hpp
    ${ENGINE_PUBLIC_INCLUDE_DIR}/Core/Camera.hpp
    ${ENGINE_PUBLIC_INCLUDE_DIR}/Core/Component.hpp
    ${ENGINE_PUBLIC_INCLUDE_DIR}/Core/Constants.hpp
    ${ENGINE_PUBLIC_INCLUDE_DIR}/Core/ECSBase.hpp
    ${ENGINE_PUBLIC_INCLUDE_DIR}/Core/GLTFLoader.hpp
    ${ENGINE_PUBLIC_INCLUDE_DIR}/Core/Mesh.hpp
    ${ENGINE_PUBLIC_INCLUDE_DIR}/Core/Object.hpp
    ${ENGINE_PUBLIC_INCLUDE_DIR}/Core/Primitive.hpp
    ${ENGINE_PUBLIC_INCLUDE_DIR}/Core/Range1D.hpp
    ${ENGINE_PUBLIC_INCLUDE_DIR}/Core/Resource.hpp
    ${ENGINE_PUBLIC_INCLUDE_DIR}/Core/Scene.hpp
    ${ENGINE_PUBLIC_INCLUDE_DIR}/Core/SceneManager.hpp
    ${ENGINE_PUBLIC_INCLUDE_DIR}/Core/SceneObject.hpp
    ${ENGINE_PUBLIC_INCLUDE_DIR}/Core/SubclassOf.hpp
    ${ENGINE_PUBLIC_INCLUDE_DIR}/Core/Texture.hpp
    ${ENGINE_PUBLIC_INCLUDE_DIR}/Core/TextureFormat.hpp
    ${ENGINE_PUBLIC_INCLUDE_DIR}/Core/Transform.hpp
    ${ENGINE_PUBLIC_INCLUDE_DIR}/Core/Type.hpp
    ${ENGINE_PUBLIC_INCLUDE_DIR}/Core/TypeRegistry.hpp
    ${ENGINE_PUBLIC_INCLUDE_DIR}/Core/TypeTraits.hpp
)

set(ENGINE_CORE_SOURCES
    ${ENGINE_SRC_DIR}/Core/BoundingBox3D.cpp
    ${ENGINE_SRC_DIR}/Core/Camera.cpp
    ${ENGINE_SRC_DIR}/Core/Component.cpp
    ${ENGINE_SRC_DIR}/Core/ECSBase.cpp
    ${ENGINE_SRC_DIR}/Core/GLTFLoader.cpp
    ${ENGINE_SRC_DIR}/Core/Material.cpp
    ${ENGINE_SRC_DIR}/Core/Mesh.cpp
    ${ENGINE_SRC_DIR}/Core/Object.cpp
    ${ENGINE_SRC_DIR}/Core/Primitive.cpp
    ${ENGINE_SRC_DIR}/Core/Range1D.cpp
    ${ENGINE_SRC_DIR}/Core/Resource.cpp
    ${ENGINE_SRC_DIR}/Core/Scene.cpp
    ${ENGINE_SRC_DIR}/Core/SceneManager.cpp
    ${ENGINE_SRC_DIR}/Core/SceneObject.cpp
    ${ENGINE_SRC_DIR}/Core/Texture.cpp
    ${ENGINE_SRC_DIR}/Core/Transform.cpp
    ${ENGINE_SRC_DIR}/Core/Type.cpp
    ${ENGINE_SRC_DIR}/Core/TypeRegistry.cpp
    ${ENGINE_SRC_DIR}/Core/TypeTraits.cpp
)

###############################################################################
# namespace Engine::Component
###############################################################################

# Definition of component public header files of the GameEngine library
# These headers will be copying alongside the library during install
set(ENGINE_COMPONENT_PUBLIC_HEADERS
    ${ENGINE_PUBLIC_INCLUDE_DIR}/Component/CameraComponent.hpp
    ${ENGINE_PUBLIC_INCLUDE_DIR}/Component/MeshComponent.hpp
)


# Definition of component system source files
set(ENGINE_COMPONENT_SOURCES
    ${ENGINE_SRC_DIR}/Component/CameraComponent.cpp
    ${ENGINE_SRC_DIR}/Component/MeshComponent.cpp
)

###############################################################################
# namespace Engine::Rendering
###############################################################################

set(ENGINE_RENDERING_PUBLIC_HEADERS
    ${ENGINE_PUBLIC_INCLUDE_DIR}/Rendering/IRenderer.hpp
    ${ENGINE_PUBLIC_INCLUDE_DIR}/Rendering/SceneView.hpp
    ${ENGINE_PUBLIC_INCLUDE_DIR}/Rendering/Shader.hpp)

set(ENGINE_RENDERING_PRIVATE_HEADERS
    ${ENGINE_PRIVATE_INCLUDE_DIR}/Rendering/DefaultRenderer.hpp
    ${ENGINE_PRIVATE_INCLUDE_DIR}/Rendering/MaterialResource.hpp
    ${ENGINE_PRIVATE_INCLUDE_DIR}/Rendering/PrimitiveResource.hpp
    ${ENGINE_PRIVATE_INCLUDE_DIR}/Rendering/RenderObject.hpp
    ${ENGINE_PRIVATE_INCLUDE_DIR}/Rendering/RenderResource.hpp
    ${ENGINE_PRIVATE_INCLUDE_DIR}/Rendering/ResourceManager.hpp
    ${ENGINE_PRIVATE_INCLUDE_DIR}/Rendering/ShaderImpl.hpp
    ${ENGINE_PRIVATE_INCLUDE_DIR}/Rendering/TextureResource.hpp
    ${ENGINE_PRIVATE_INCLUDE_DIR}/Rendering/TransformResource.hpp
)

set(ENGINE_RENDERING_SOURCES
    ${ENGINE_SRC_DIR}/Rendering/DefaultRenderer.cpp
    ${ENGINE_SRC_DIR}/Rendering/MaterialResource.cpp
    ${ENGINE_SRC_DIR}/Rendering/PrimitiveResource.cpp
    ${ENGINE_SRC_DIR}/Rendering/RenderObject.cpp
    ${ENGINE_SRC_DIR}/Rendering/RenderResource.cpp
    ${ENGINE_SRC_DIR}/Rendering/ResourceManager.cpp
    ${ENGINE_SRC_DIR}/Rendering/ShaderImpl.cpp
    ${ENGINE_SRC_DIR}/Rendering/TextureResource.cpp
    ${ENGINE_SRC_DIR}/Rendering/TransformResource.cpp
)

# Compile the Default PBR Shader
target_compile_shader(${ENGINE_TARGET_NAME} 
    NAME DefaultPBR
    NAMESPACE Shaders
    VERTEX_SHADER
        ${ENGINE_SRC_DIR}/Rendering/Shaders/DefaultPBR.vertex.glsl
    FRAGMENT_SHADER
        ${ENGINE_SRC_DIR}/Rendering/Shaders/DefaultPBR.fragment.glsl
    OUTPUT
        ${ENGINE_GENERATED_SHADER_DIR}/Engine/Rendering/Shaders/DefaultPBR.hpp
    WORKING_DIRECTORY
        ${ENGINE_PUBLIC_INCLUDE_DIR}/Rendering/Shaders/)

set (ENGINE_RENDERING_PUBLIC_SHADERS
    ${ENGINE_PUBLIC_INCLUDE_DIR}/Rendering/Shaders/DefaultUniforms.h
    ${ENGINE_PUBLIC_INCLUDE_DIR}/Rendering/Shaders/MaterialBase.vertex.glsl
    ${ENGINE_PUBLIC_INCLUDE_DIR}/Rendering/Shaders/MaterialBase.fragment.glsl
    ${ENGINE_PUBLIC_INCLUDE_DIR}/Rendering/Shaders/Common.glsl)

set(ENGINE_RENDERING_SHADERS

    ${ENGINE_SRC_DIR}/Rendering/Shaders/DefaultPBR.vertex.glsl
    ${ENGINE_SRC_DIR}/Rendering/Shaders/DefaultPBR.fragment.glsl)

set(ENGINE_RENDERING_GENERATED_SHADER_HEADERS
    ${ENGINE_GENERATED_SHADER_DIR}/Engine/Rendering/Shaders/DefaultPBR.hpp)

###############################################################################
# Link the Engine library with the following libraries
###############################################################################

find_package(glfw3)
find_package(glm)
find_package(TinyGLTF)
find_package(stb REQUIRED)
find_package(imgui REQUIRED)
find_package(nlohmann_json REQUIRED)

###############################################################################
# Create the Engine library
###############################################################################

coral_configure_target(${ENGINE_TARGET_NAME}
          PUBLIC_HEADERS
            ${ENGINE_APPLICATION_PUBLIC_HEADERS}
            ${ENGINE_COMPONENT_PUBLIC_HEADERS}
            ${ENGINE_CORE_PUBLIC_HEADERS}
            ${ENGINE_RENDERING_PUBLIC_HEADERS}
            ${ENGINE_RENDERING_PUBLIC_SHADERS}
          PRIVATE_HEADERS
            ${ENGINE_APPLICATION_PRIVATE_HEADERS}
            ${ENGINE_RENDERING_GENERATED_SHADER_HEADERS}
            ${ENGINE_RENDERING_PRIVATE_HEADERS}
          SOURCES
            ${ENGINE_APPLICATION_SOURCES}
            ${ENGINE_COMPONENT_SOURCES}
            ${ENGINE_CORE_SOURCES}
            ${ENGINE_RENDERING_SOURCES}
            ${ENGINE_RENDERING_SHADERS}
          PUBLIC_DEPENDENCIES
            imgui::imgui
            TinyGLTF::TinyGLTF
            nlohmann_json::nlohmann_json
            glm::glm
            Common
            Log20
            Coral
          PRIVATE_DEPENDENCIES
            stb::stb
            glfw::glfw)

###############################################################################
# Organize files in the IDE
###############################################################################

source_group("public/include/Engine/Application/" FILES  ${ENGINE_APPLICATION_PUBLIC_HEADERS})
source_group("private/include/Engine/Application/" FILES ${ENGINE_APPLICATION_PRIVATE_HEADERS})
source_group("private/src/Engine/Application/" FILES ${ENGINE_APPLICATION_SOURCES})

source_group("public/include/Engine/Core/" FILES ${ENGINE_CORE_PUBLIC_HEADERS})
source_group("private/include/Engine/Core/" FILES ${ENGINE_CORE_PRIVATE_HEADERS})
source_group("private/src/Engine/Core/" FILES ${ENGINE_CORE_SOURCES})

source_group("public/include/Engine/Rendering/" FILES ${ENGINE_RENDERING_PUBLIC_HEADERS})
source_group("public/include/Engine/Rendering/Shaders" FILES ${ENGINE_RENDERING_PUBLIC_SHADERS})
source_group("private/include/Engine/Rendering/" FILES ${ENGINE_RENDERING_PRIVATE_HEADERS})
source_group("private/src/Engine/Rendering/" FILES ${ENGINE_RENDERING_SOURCES})
source_group("private/generated/include/Engine/Rendering/Shaders/" FILES ${ENGINE_RENDERING_GENERATED_SHADER_HEADERS})
source_group("private/src/Engine/Rendering/Shaders/" FILES ${ENGINE_RENDERING_SHADERS})

source_group("public/include/Engine/Component/" FILES ${ENGINE_COMPONENT_PUBLIC_HEADERS})
source_group("private/src/Engine/Component/" FILES ${ENGINE_COMPONENT_SOURCES})
