################################################################################
# This CMakeLists.txt contains the build configuration for the Engine 
# library. It contains the library definition and install configuration of the
################################################################################

# The name of the GameEngine library
set(ENGINE_TARGET_NAME Engine)

# The source location of the public Engine headers
set(ENGINE_PUBLIC_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/public/include/${ENGINE_TARGET_NAME})

# The source location of the private Engine headers
set(ENGINE_PRIVATE_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/private/include/${ENGINE_TARGET_NAME})

# The source location of the Engine library
set(ENGINE_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/private/src)

set(ENGINE_RENDERING_SHADER_DIR ${ENGINE_SRC_DIR}/Rendering/Shaders)

set(ENGINE_GENERATED_SHADER_DIR ${CMAKE_CURRENT_BINARY_DIR}/generated/private/include)

message(WARNING "ENGINE_GENERATED_SHADER_DIR=${ENGINE_GENERATED_SHADER_DIR}")
set(ENGINE_APPLICATION_PUBLIC_MODULES
    ${ENGINE_PUBLIC_INCLUDE_DIR}/Application/Application.ixx
    ${ENGINE_PUBLIC_INCLUDE_DIR}/Application/CommandLineArguments.ixx
    ${ENGINE_PUBLIC_INCLUDE_DIR}/Application/Layer.ixx
    ${ENGINE_PUBLIC_INCLUDE_DIR}/Application/WindowLayer.ixx
    ${ENGINE_PUBLIC_INCLUDE_DIR}/Application/Window.ixx)

set(ENGINE_APPLICATION_PUBLIC_HEADERS
    ${ENGINE_PUBLIC_INCLUDE_DIR}/EntryPoint.h)

set(ENGINE_APPLICATION_PRIVATE_MODULES
    ${ENGINE_PRIVATE_INCLUDE_DIR}/Application/ApplicationImpl.ixx
    ${ENGINE_PRIVATE_INCLUDE_DIR}/Application/WindowImpl.ixx)

set(ENGINE_APPLICATION_SOURCES
    ${ENGINE_SRC_DIR}/Application/Application.cpp
    ${ENGINE_SRC_DIR}/Application/ApplicationImpl.cpp
    ${ENGINE_SRC_DIR}/Application/Window.cpp
    ${ENGINE_SRC_DIR}/Application/WindowImpl.cpp
    ${ENGINE_SRC_DIR}/Application/WindowLayer.cpp
    ${ENGINE_SRC_DIR}/Application/Layer.cpp
    ${ENGINE_SRC_DIR}/Application/CommandLineArguments.cpp)

set(ENGINE_CORE_PUBLIC_MODULES
	${ENGINE_PUBLIC_INCLUDE_DIR}/Core/Material.ixx
    ${ENGINE_PUBLIC_INCLUDE_DIR}/Core/BoundingBox3D.ixx
    ${ENGINE_PUBLIC_INCLUDE_DIR}/Core/Camera.ixx
    #${ENGINE_PUBLIC_INCLUDE_DIR}/Core/DirectionalLight.ixx

    ${ENGINE_PUBLIC_INCLUDE_DIR}/Core/Time.ixx
    ${ENGINE_PUBLIC_INCLUDE_DIR}/Core/ResourceManager.ixx
    ${ENGINE_PUBLIC_INCLUDE_DIR}/Core/SceneManager.ixx
    ${ENGINE_PUBLIC_INCLUDE_DIR}/Core/Constants.ixx
    ${ENGINE_PUBLIC_INCLUDE_DIR}/Core/Event.ixx
    ${ENGINE_PUBLIC_INCLUDE_DIR}/Core/GameObject.ixx
    ${ENGINE_PUBLIC_INCLUDE_DIR}/Core/GameObjectFactory.ixx
    ${ENGINE_PUBLIC_INCLUDE_DIR}/Core/Image.ixx
    ${ENGINE_PUBLIC_INCLUDE_DIR}/Core/ImageLoader.ixx
    ${ENGINE_PUBLIC_INCLUDE_DIR}/Core/SubclassOf.ixx
    ${ENGINE_PUBLIC_INCLUDE_DIR}/Core/ISerializable.ixx

    #${ENGINE_PUBLIC_INCLUDE_DIR}/Core/Light.ixx
    ${ENGINE_PUBLIC_INCLUDE_DIR}/Core/Mesh.ixx
    ${ENGINE_PUBLIC_INCLUDE_DIR}/Core/MeshFactory.ixx
    ${ENGINE_PUBLIC_INCLUDE_DIR}/Core/Object.ixx

    ${ENGINE_PUBLIC_INCLUDE_DIR}/Core/Primitive.ixx
    #${ENGINE_PUBLIC_INCLUDE_DIR}/Property.ixx
    ${ENGINE_PUBLIC_INCLUDE_DIR}/Core/Range1D.ixx
    ${ENGINE_PUBLIC_INCLUDE_DIR}/Core/Scene.ixx
    #${ENGINE_PUBLIC_INCLUDE_DIR}/Serializer.ixx
    ${ENGINE_PUBLIC_INCLUDE_DIR}/Core/Shader.ixx
    ${ENGINE_PUBLIC_INCLUDE_DIR}/Core/ShaderManager.ixx
    #${ENGINE_PUBLIC_INCLUDE_DIR}/SkyLight.ixx
    ${ENGINE_PUBLIC_INCLUDE_DIR}/Core/Texture.ixx
    ${ENGINE_PUBLIC_INCLUDE_DIR}/Core/TextureFactory.ixx
    ${ENGINE_PUBLIC_INCLUDE_DIR}/Core/TextureFormat.ixx
    ${ENGINE_PUBLIC_INCLUDE_DIR}/Core/Transform.ixx
    ${ENGINE_PUBLIC_INCLUDE_DIR}/Core/Type.ixx
    ${ENGINE_PUBLIC_INCLUDE_DIR}/Core/TypeTraits.ixx
    ${ENGINE_PUBLIC_INCLUDE_DIR}/Core/TypeRegistry.ixx
)

set(ENGINE_CORE_PRIVATE_MODULES
	${ENGINE_PRIVATE_INCLUDE_DIR}/Core/ShaderManagerImpl.ixx
    ${ENGINE_PRIVATE_INCLUDE_DIR}/Core/ShaderImpl.ixx)

set(ENGINE_CORE_SOURCES
    ${ENGINE_SRC_DIR}/Core/BoundingBox3D.cpp
    ${ENGINE_SRC_DIR}/Core/Camera.cpp
    #${ENGINE_SRC_DIR}/DirectionalLight.cpp
    #${ENGINE_SRC_DIR}/Engine.cpp
    ${ENGINE_SRC_DIR}/Core/Time.cpp
    ${ENGINE_SRC_DIR}/Core/ResourceManager.cpp
    ${ENGINE_SRC_DIR}/Core/SceneManager.cpp
    ${ENGINE_SRC_DIR}/Core/GameObject.cpp
    ${ENGINE_SRC_DIR}/Core/GameObjectFactory.cpp
    ${ENGINE_SRC_DIR}/Core/Image.cpp
    ${ENGINE_SRC_DIR}/Core/ImageLoader.cpp
    #${ENGINE_SRC_DIR}/Light.cpp
    ${ENGINE_SRC_DIR}/Core/Material.cpp
    ${ENGINE_SRC_DIR}/Core/Mesh.cpp
    ${ENGINE_SRC_DIR}/Core/MeshFactory.cpp
    ${ENGINE_SRC_DIR}/Core/Object.cpp
    #${ENGINE_SRC_DIR}/ObjectFactory.cpp
    #${ENGINE_SRC_DIR}/ObjectFactory.ixx
    ${ENGINE_SRC_DIR}/Core/Primitive.cpp
    ${ENGINE_SRC_DIR}/Core/Range1D.cpp
    ${ENGINE_SRC_DIR}/Core/Scene.cpp

    ${ENGINE_SRC_DIR}/Core/ShaderImpl.cpp
    ${ENGINE_SRC_DIR}/Core/ShaderManagerImpl.cpp
    #${ENGINE_SRC_DIR}/SkyLight.cpp
    ${ENGINE_SRC_DIR}/Core/Texture.cpp
    ${ENGINE_SRC_DIR}/Core/TextureFactory.cpp
    ${ENGINE_SRC_DIR}/Core/Transform.cpp
    ${ENGINE_SRC_DIR}/Core/Type.cpp
    ${ENGINE_SRC_DIR}/Core/TypeRegistry.cpp
    ${ENGINE_SRC_DIR}/Core/TypeTraits.cpp
)


# Definition of component public header files of the GameEngine library
# These headers will be copying alongside the library during install
set(ENGINE_COMPONENT_PUBLIC_MODULES
    ${ENGINE_PUBLIC_INCLUDE_DIR}/Component/CameraComponent.ixx
    ${ENGINE_PUBLIC_INCLUDE_DIR}/Component/Component.ixx
    #${ENGINE_PUBLIC_INCLUDE_DIR}/Component/DirectionalLightComponent.ixx
    #${ENGINE_PUBLIC_INCLUDE_DIR}/Component/LightComponent.ixx
    ${ENGINE_PUBLIC_INCLUDE_DIR}/Component/MeshComponent.ixx
    #${ENGINE_PUBLIC_INCLUDE_DIR}/Component/PointLightComponent.ixx
    #${ENGINE_PUBLIC_INCLUDE_DIR}/Component/SkyLightComponent.ixx
    #${ENGINE_PUBLIC_INCLUDE_DIR}/Component/SpotLightComponent.ixx
)


# Definition of component system source files
set(ENGINE_COMPONENT_SOURCES
    ${ENGINE_SRC_DIR}/Component/CameraComponent.cpp
    ${ENGINE_SRC_DIR}/Component/Component.cpp
    #${ENGINE_SRC_DIR}/Component/DirectionalLightComponent.cpp
    #${ENGINE_SRC_DIR}/Component/LightComponent.cpp
    ${ENGINE_SRC_DIR}/Component/MeshComponent.cpp
    #${ENGINE_SRC_DIR}/Component/PointLightComponent.cpp
    #${ENGINE_SRC_DIR}/Component/SkyLightComponent.cpp
    #${ENGINE_SRC_DIR}/Component/SpotLightComponent.cpp
)

set(ENGINE_RENDERING_PUBLIC_MODULES
    ${ENGINE_PUBLIC_INCLUDE_DIR}/Rendering/IRenderer.ixx
    ${ENGINE_PUBLIC_INCLUDE_DIR}/Rendering/RenderObject.ixx)

set(ENGINE_RENDERING_PRIVATE_MODULES
    ${ENGINE_PRIVATE_INCLUDE_DIR}/Rendering/SceneRenderer.ixx
    ${ENGINE_PRIVATE_INCLUDE_DIR}/Rendering/DefaultRenderer.ixx
    ${ENGINE_PRIVATE_INCLUDE_DIR}/Rendering/RenderGraph.ixx
    ${ENGINE_PRIVATE_INCLUDE_DIR}/Rendering/SceneView.ixx
    ${ENGINE_PRIVATE_INCLUDE_DIR}/Rendering/PrimitiveResource.ixx
    ${ENGINE_PRIVATE_INCLUDE_DIR}/Rendering/ResourceManager.ixx)


# Definition of rendering source files
set(ENGINE_RENDERING_SOURCES
    ${ENGINE_SRC_DIR}/Rendering/DefaultRenderer.cpp
#    ${ENGINE_SRC_DIR}/Rendering/CalculateIrradianceRenderer.ixx
#    ${ENGINE_SRC_DIR}/Rendering/DebugRenderer.cpp
#    ${ENGINE_SRC_DIR}/Rendering/DebugRenderer.ixx
#    ${ENGINE_SRC_DIR}/Rendering/DefaultShaders.ixx
#    ${ENGINE_SRC_DIR}/Rendering/DefaultShaders.cpp
#    ${ENGINE_SRC_DIR}/Rendering/DirectionalLightRenderer.cpp
#    ${ENGINE_SRC_DIR}/Rendering/DirectionalLightRenderer.ixx
#    ${ENGINE_SRC_DIR}/Rendering/DirectionalLightResource.cpp
#    ${ENGINE_SRC_DIR}/Rendering/DirectionalLightResource.ixx
#    ${ENGINE_SRC_DIR}/Rendering/FullscreenTechnique.cpp
#    ${ENGINE_SRC_DIR}/Rendering/FullscreenTechnique.ixx
#    ${ENGINE_SRC_DIR}/Rendering/FXAATechnique.cpp
#    ${ENGINE_SRC_DIR}/Rendering/FXAATechnique.ixx
#    ${ENGINE_SRC_DIR}/Rendering/GammaCorrectionTechnique.cpp
#    ${ENGINE_SRC_DIR}/Rendering/GammaCorrectionTechnique.ixx
#    ${ENGINE_SRC_DIR}/Rendering/GBuffer.cpp
#    ${ENGINE_SRC_DIR}/Rendering/GBuffer.ixx
#    ${ENGINE_SRC_DIR}/Rendering/GeometryRenderer.cpp
#    ${ENGINE_SRC_DIR}/Rendering/GeometryRenderer.ixx
#    ${ENGINE_SRC_DIR}/Rendering/LightingPass.cpp
#    ${ENGINE_SRC_DIR}/Rendering/LightingPass.ixx
#    ${ENGINE_SRC_DIR}/Rendering/MaterialResource.cpp
#    ${ENGINE_SRC_DIR}/Rendering/MaterialResource.ixx
#    ${ENGINE_SRC_DIR}/Rendering/PostProcessingPass.cpp
#    ${ENGINE_SRC_DIR}/Rendering/PostProcessingPass.ixx
#    ${ENGINE_SRC_DIR}/Rendering/PostprocessingTechnique.cpp
#    ${ENGINE_SRC_DIR}/Rendering/PostprocessingTechnique.ixx
    ${ENGINE_SRC_DIR}/Rendering/PrimitiveResource.cpp
#    ${ENGINE_SRC_DIR}/Rendering/PrimitiveResource.ixx
#    ${ENGINE_SRC_DIR}/Rendering/Renderer.cpp
#    ${ENGINE_SRC_DIR}/Rendering/Renderer.ixx
#    ${ENGINE_SRC_DIR}/Rendering/RenderResource.cpp
#    ${ENGINE_SRC_DIR}/Rendering/RenderResource.ixx
#    ${ENGINE_SRC_DIR}/Rendering/ResourceManager.cpp
#    ${ENGINE_SRC_DIR}/Rendering/ResourceManager.ixx
#    ${ENGINE_SRC_DIR}/Rendering/SceneRenderer.cpp
#    ${ENGINE_SRC_DIR}/Rendering/SceneRenderer.ixx
#    ${ENGINE_SRC_DIR}/Rendering/SceneView.ixx
     ${ENGINE_SRC_DIR}/Rendering/ShaderResource.cpp

#    ${ENGINE_SRC_DIR}/Rendering/ShaderManager.cpp
#    ${ENGINE_SRC_DIR}/Rendering/ShaderManager.ixx
#    ${ENGINE_SRC_DIR}/Rendering/ShadowMapRenderer.cpp
#    ${ENGINE_SRC_DIR}/Rendering/ShadowMapRenderer.ixx
#    ${ENGINE_SRC_DIR}/Rendering/SkyLightRenderer.cpp
#    ${ENGINE_SRC_DIR}/Rendering/SkyLightRenderer.ixx
#    ${ENGINE_SRC_DIR}/Rendering/SkyLightResource.cpp
#    ${ENGINE_SRC_DIR}/Rendering/SkyLightResource.ixx
#    ${ENGINE_SRC_DIR}/Rendering/SSAORenderer.cpp
#    ${ENGINE_SRC_DIR}/Rendering/SSAORenderer.ixx
    ${ENGINE_SRC_DIR}/Rendering/TextureResource.cpp
#    ${ENGINE_SRC_DIR}/Rendering/ToneMappingTechnique.cpp
#    ${ENGINE_SRC_DIR}/Rendering/ToneMappingTechnique.ixx
#    ${ENGINE_SRC_DIR}/Rendering/UniformBufferFactory.cpp
#    ${ENGINE_SRC_DIR}/Rendering/UniformBufferFactory.ixx
#    ${ENGINE_SRC_DIR}/Rendering/IRenderer.ixx
)


# Compile the Selection Shader
#compileShader(Selection Shaders
#              ${ENGINE_GENERATED_INCLUDE_DIR}/Shaders/Selection.h
#              ${ENGINE_SHADER_DIR}/FullscreenQuad.vertex.glsl
#              ${ENGINE_SHADER_DIR}/Selection.fragment.glsl
#              ${ENGINE_SHADER_DIR})
#
## Compile the Uniform Color Shader
#compileShader(UniformColor Shaders
#              ${ENGINE_GENERATED_INCLUDE_DIR}/Shaders/UniformColor.h
#              ${ENGINE_SHADER_DIR}/DepthOnly.vertex.glsl
#              ${ENGINE_SHADER_DIR}/UniformColor.fragment.glsl
#              ${ENGINE_SHADER_DIR})
#

# Compile the Default PBR Shader
compileShader(DefaultPBR Shaders
              ${ENGINE_GENERATED_SHADER_DIR}/Rendering/Shaders/DefaultPBR.h
              ${ENGINE_PRIVATE_INCLUDE_DIR}/Rendering/Shaders/DefaultPBR.vertex.glsl
              ${ENGINE_PRIVATE_INCLUDE_DIR}/Rendering/Shaders/DefaultPBR.fragment.glsl
              ${ENGINE_PUBLIC_INCLUDE_DIR}/Rendering/Shaders/)

set(ENGINE_RENDERING_GENERATED_SHADER_HEADERS
    ${ENGINE_GENERATED_SHADER_DIR}/Rendering/Shaders/DefaultPBR.h)

set(ENGINE_RENDERING_PUBLIC_SHADER_SOURCES
    ${ENGINE_PUBLIC_INCLUDE_DIR}/Rendering/Shaders/Common.glsl
    ${ENGINE_PUBLIC_INCLUDE_DIR}/Rendering/Shaders/PBRTemplate.vertex.glsl
    ${ENGINE_PUBLIC_INCLUDE_DIR}/Rendering/Shaders/PBRTemplate.fragment.glsl)

set(ENGINE_RENDERING_PRIVATE_SHADER_SOURCES
    ${ENGINE_PRIVATE_INCLUDE_DIR}/Rendering/Shaders/DefaultPBR.vertex.glsl
    ${ENGINE_PRIVATE_INCLUDE_DIR}/Rendering/Shaders/DefaultPBR.fragment.glsl)

## Compile the SSAO Shader
#compileShader(SSAO Shaders
#              ${ENGINE_GENERATED_INCLUDE_DIR}/Shaders/SSAO.h
#              ${ENGINE_SHADER_DIR}/FullscreenQuad.vertex.glsl
#              ${ENGINE_SHADER_DIR}/SSAO.fragment.glsl
#              ${ENGINE_SHADER_DIR})
#
## Compile the SkyboxShader
#compileShader(UnlitSkybox Shaders
#              ${ENGINE_GENERATED_INCLUDE_DIR}/Shaders/UnlitSkyboxShader.h
#              ${ENGINE_SHADER_DIR}/FullscreenQuad.vertex.glsl
#              ${ENGINE_SHADER_DIR}/UnlitSkybox.fragment.glsl
#              ${ENGINE_SHADER_DIR})
#
## Compile the Directional Light shader
#compileShader(DirectionalLightPass Shaders
#              ${ENGINE_GENERATED_INCLUDE_DIR}/Shaders/DirectionalLightShader.h
#              ${ENGINE_SHADER_DIR}/FullscreenQuad.vertex.glsl
#              ${ENGINE_SHADER_DIR}/DirectionalLight.fragment.glsl
#              ${ENGINE_SHADER_DIR})
#
## Compile the Environment Light shader
#compileShader(EnvironmentLightShader Shaders
#              ${ENGINE_GENERATED_INCLUDE_DIR}/Shaders/EnvironmentLightShader.h
#              ${ENGINE_SHADER_DIR}/FullscreenQuad.vertex.glsl
#              ${ENGINE_SHADER_DIR}/EnvironmentLight.fragment.glsl
#              ${ENGINE_SHADER_DIR})
#
## Compile the Irradiance map Shader
#compileShader(CalculateIrradianceShader Shaders
#              ${ENGINE_GENERATED_INCLUDE_DIR}/Shaders/CalculateIrradianceShader.h
#              ${ENGINE_SHADER_DIR}/FullscreenQuad.vertex.glsl
#              ${ENGINE_SHADER_DIR}/CalculateIrradiance.fragment.glsl
#              ${ENGINE_SHADER_DIR})
#
## Compile the Average Blur Shader
#compileShader(AverageBlur Shaders
#              ${ENGINE_GENERATED_INCLUDE_DIR}/Shaders/AverageBlurShader.h
#              ${ENGINE_SHADER_DIR}/FullscreenQuad.vertex.glsl
#              ${ENGINE_SHADER_DIR}/AverageBlur.fragment.glsl
#              ${ENGINE_SHADER_DIR})
#
## Compile the Depth Shader
#compileShader(DepthShader Shaders
#              ${ENGINE_GENERATED_INCLUDE_DIR}/Shaders/DepthShader.h
#              ${ENGINE_SHADER_DIR}/DepthOnly.vertex.glsl
#              ${ENGINE_SHADER_DIR}/DepthOnly.fragment.glsl
#              ${ENGINE_SHADER_DIR})
#
## Compile the Tone Mapping Shader
#compileShader(ToneMappingShader Shaders
#              ${ENGINE_GENERATED_INCLUDE_DIR}/Shaders/ToneMappingShader.h
#              ${ENGINE_SHADER_DIR}/FullscreenQuad.vertex.glsl
#              ${ENGINE_SHADER_DIR}/ToneMapping.fragment.glsl
#              ${ENGINE_SHADER_DIR})
#
## Compile the Gamma Correction Shader
#compileShader(GammaCorrectionShader Shaders
#              ${ENGINE_GENERATED_INCLUDE_DIR}/Shaders/GammaCorrectionShader.h
#              ${ENGINE_SHADER_DIR}/FullscreenQuad.vertex.glsl
#              ${ENGINE_SHADER_DIR}/GammaCorrection.fragment.glsl
#              ${ENGINE_SHADER_DIR})

# Compile the FXAA Shader
#compileShader(FXAAShader Shaders
#              ${ENGINE_GENERATED_INCLUDE_DIR}/Shaders/FXAAShader.h
#              ${ENGINE_SHADER_DIR}/FullscreenQuad.vertex.glsl
#              ${ENGINE_SHADER_DIR}/FXAA.fragment.glsl
#              ${ENGINE_SHADER_DIR})

#set(ENGINE_RENDERING_SHADER_SOURCES
#    ${ENGINE_SHADER_DIR}/AverageBlur.fragment.glsl
#    ${ENGINE_SHADER_DIR}/Common.glsl
#    ${ENGINE_SHADER_DIR}/PBRTemplate.fragment.glsl
#    ${ENGINE_SHADER_DIR}/PBRTemplate.vertex.glsl
#    ${ENGINE_SHADER_DIR}/CalculateIrradiance.fragment.glsl
#    ${ENGINE_SHADER_DIR}/DefaultPBR.fragment.glsl
#    ${ENGINE_SHADER_DIR}/DefaultPBR.vertex.glsl
#    ${ENGINE_SHADER_DIR}/TexturedPBR.fragment.glsl
#    ${ENGINE_SHADER_DIR}/TexturedPBR.vertex.glsl
#    ${ENGINE_SHADER_DIR}/DepthOnly.fragment.glsl
#    ${ENGINE_SHADER_DIR}/DepthOnly.vertex.glsl
#    ${ENGINE_SHADER_DIR}/DirectionalLight.fragment.glsl
#    ${ENGINE_SHADER_DIR}/EnvironmentLight.fragment.glsl    
#    ${ENGINE_SHADER_DIR}/FullscreenQuad.vertex.glsl
#    ${ENGINE_SHADER_DIR}/FXAA.fragment.glsl
#    ${ENGINE_SHADER_DIR}/GammaCorrection.fragment.glsl
#    ${ENGINE_SHADER_DIR}/Selection.fragment.glsl
#    ${ENGINE_SHADER_DIR}/SphericalCoordinates.glsl
#    ${ENGINE_SHADER_DIR}/SSAO.fragment.glsl
#    ${ENGINE_SHADER_DIR}/ToneMapping.fragment.glsl
#    ${ENGINE_SHADER_DIR}/UniformColor.fragment.glsl
#    ${ENGINE_SHADER_DIR}/UnlitSkybox.fragment.glsl
#)

#set(ENGINE_RENDERING_SHADERS_GENERATED
#    ${ENGINE_GENERATED_INCLUDE_DIR}/Shaders/AverageBlurShader.h
#    ${ENGINE_GENERATED_INCLUDE_DIR}/Shaders/CalculateIrradianceShader.h
#    ${ENGINE_GENERATED_INCLUDE_DIR}/Shaders/DepthShader.h
#    ${ENGINE_GENERATED_INCLUDE_DIR}/Shaders/DirectionalLightShader.h
#    ${ENGINE_GENERATED_INCLUDE_DIR}/Shaders/EnvironmentLightShader.h
#    ${ENGINE_GENERATED_INCLUDE_DIR}/Shaders/FXAAShader.h
#    ${ENGINE_GENERATED_INCLUDE_DIR}/Shaders/GammaCorrectionShader.h
#    ${ENGINE_GENERATED_INCLUDE_DIR}/Shaders/GeometryPassShader.h
#    ${ENGINE_GENERATED_INCLUDE_DIR}/Shaders/ToneMappingShader.h
#    ${ENGINE_GENERATED_INCLUDE_DIR}/Shaders/UnlitSkyboxShader.h
#    ${ENGINE_GENERATED_INCLUDE_DIR}/Shaders/DefaultPBR.h
#    ${ENGINE_GENERATED_INCLUDE_DIR}/Shaders/TexturedPBR.h
#)

###############################################################################
# Target-specific definition of static or shared build
###############################################################################

option(ENGINE_BUILD_SHARED_LIBRARY "Build and link ${ENGINE_TARGET_NAME} as a shared library" OFF)

if (ENGINE_BUILD_SHARED_LIBRARY)
    set(ENGINE_BUILD_TYPE SHARED_LIBRARY)
else()
    set(ENGINE_BUILD_TYPE STATIC_LIBRARY)
endif()

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

AddTarget(TARGET_NAME ${ENGINE_TARGET_NAME}
          TARGET_TYPE ${ENGINE_BUILD_TYPE}
          PUBLIC_MODULES 
            ${ENGINE_APPLICATION_PUBLIC_MODULES}
            ${ENGINE_CORE_PUBLIC_MODULES}
            ${ENGINE_COMPONENT_PUBLIC_MODULES}
            ${ENGINE_RENDERING_PUBLIC_MODULES}
          PUBLIC_HEADERS 
            ${ENGINE_APPLICATION_PUBLIC_HEADERS}
          PRIVATE_HEADERS 
            ${ENGINE_RENDERING_GENERATED_SHADER_HEADERS}
          PRIVATE_MODULES 
            ${ENGINE_APPLICATION_PRIVATE_MODULES}  
            ${ENGINE_CORE_PRIVATE_MODULES}
            ${ENGINE_RENDERING_PRIVATE_MODULES}
          MODULE_SOURCES 
            ${ENGINE_APPLICATION_SOURCES}
            ${ENGINE_CORE_SOURCES}
            ${ENGINE_COMPONENT_SOURCES}
            ${ENGINE_RENDERING_SOURCES}
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


target_sources(${ENGINE_TARGET_NAME} PRIVATE ${ENGINE_RENDERING_PUBLIC_SHADER_SOURCES} ${ENGINE_RENDERING_PRIVATE_SHADER_SOURCES})

###############################################################################
# Organize files in the IDE
###############################################################################

source_group("include/public/Engine/Application/" FILES ${ENGINE_APPLICATION_PUBLIC_MODULES} ${ENGINE_APPLICATION_PUBLIC_HEADERS})
source_group("include/private/Engine/Application/" FILES ${ENGINE_APPLICATION_PRIVATE_MODULES})
source_group("src/Engine/Application/" FILES ${ENGINE_APPLICATION_SOURCES})

source_group("include/public/Engine/Core/" FILES ${ENGINE_CORE_PUBLIC_MODULES})
source_group("include/private/Engine/Core/" FILES ${ENGINE_CORE_PRIVATE_MODULES})
source_group("src/Engine/Core/" FILES ${ENGINE_CORE_SOURCES})

source_group("include/public/Engine/Component/" FILES ${ENGINE_COMPONENT_PUBLIC_MODULES})
source_group("src/Engine/Component/" FILES ${ENGINE_COMPONENT_SOURCES})

source_group("include/public/Engine/Rendering/" FILES ${ENGINE_RENDERING_PUBLIC_MODULES})
source_group("include/private/Engine/Rendering/" FILES ${ENGINE_RENDERING_PRIVATE_MODULES})
source_group("src/Engine/Rendering/" FILES ${ENGINE_RENDERING_SOURCES})

source_group("include/public/Engine/Rendering/Shaders" FILES ${ENGINE_RENDERING_PUBLIC_SHADER_SOURCES})
source_group("include/private/Engine/Rendering/Shaders" FILES ${ENGINE_RENDERING_PRIVATE_SHADER_SOURCES})
source_group("include/private/Engine/Rendering/Shaders/generated" FILES ${ENGINE_RENDERING_GENERATED_SHADER_HEADERS})
