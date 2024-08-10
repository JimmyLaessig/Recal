###############################################################################
# This file contains the build configuration for the Coral library
###############################################################################

# The name of the library
set(CORAL_TARGET_NAME Coral)

###############################################################################
# Build the library as static or shared
###############################################################################

option(CORAL_BUILD_SHARED_LIBRARY "Build and link ${CORAL_TARGET_NAME} as a shared library" OFF)

if (CORAL_BUILD_SHARED_LIBRARY)
    set(CORAL_TARGET_TYPE SHARED_LIBRARY)
else()
    set(CORAL_TARGET_TYPE STATIC_LIBRARY)
endif()

###############################################################################
# Add the source files to the library
###############################################################################

# Location of the Coral library source files
set(CORAL_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

# Location of the Coral library include files
set(CORAL_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include/${CORAL_TARGET_NAME})

# Definition of all public module files of the Coral library
set(CORAL_PUBLIC_MODULES
    ${CORAL_INCLUDE_DIR}/Buffer.ixx
    ${CORAL_INCLUDE_DIR}/BufferView.ixx
	${CORAL_INCLUDE_DIR}/Context.ixx
    ${CORAL_INCLUDE_DIR}/CommandBuffer.ixx
    ${CORAL_INCLUDE_DIR}/CommandQueue.ixx
    ${CORAL_INCLUDE_DIR}/DescriptorSet.ixx
    ${CORAL_INCLUDE_DIR}/Fence.ixx
    ${CORAL_INCLUDE_DIR}/RAII.ixx
    ${CORAL_INCLUDE_DIR}/Coral.ixx
    ${CORAL_INCLUDE_DIR}/Framebuffer.ixx
    ${CORAL_INCLUDE_DIR}/PipelineState.ixx
    ${CORAL_INCLUDE_DIR}/Sampler.ixx
    ${CORAL_INCLUDE_DIR}/ShaderModule.ixx
    ${CORAL_INCLUDE_DIR}/Semaphore.ixx
    ${CORAL_INCLUDE_DIR}/Surface.ixx
    ${CORAL_INCLUDE_DIR}/Types.ixx
    ${CORAL_INCLUDE_DIR}/Image.ixx
    ${CORAL_INCLUDE_DIR}/UniformBlockBuilder.ixx
)

# Definition of all Vulkan module files of the Coral library
set(CORAL_PRIVATE_MODULES_VULKAN
    ${CORAL_SRC_DIR}/Vulkan/Buffer.ixx
    ${CORAL_SRC_DIR}/Vulkan/BufferView.ixx
    ${CORAL_SRC_DIR}/Vulkan/CommandBuffer.ixx
    ${CORAL_SRC_DIR}/Vulkan/CommandQueue.ixx
    ${CORAL_SRC_DIR}/Vulkan/DescriptorSet.ixx
    ${CORAL_SRC_DIR}/Vulkan/Context.ixx
    ${CORAL_SRC_DIR}/Vulkan/Fence.ixx
    ${CORAL_SRC_DIR}/Vulkan/Format.ixx
    ${CORAL_SRC_DIR}/Vulkan/Image.ixx
    ${CORAL_SRC_DIR}/Vulkan/PipelineState.ixx
    ${CORAL_SRC_DIR}/Vulkan/Framebuffer.ixx
    ${CORAL_SRC_DIR}/Vulkan/Sampler.ixx
    ${CORAL_SRC_DIR}/Vulkan/Semaphore.ixx
    ${CORAL_SRC_DIR}/Vulkan/ShaderModule.ixx
    ${CORAL_SRC_DIR}/Vulkan/SurfaceWin32.ixx
    ${CORAL_SRC_DIR}/Vulkan/Surface.ixx
    ${CORAL_SRC_DIR}/Vulkan/BufferPool.ixx
)

# Definition of all private header files of the Coral library
set(CORAL_PRIVATE_MODULES
    ${CORAL_SRC_DIR}/Factory.ixx
)

# Definition of all private header files of the Coral library
set(CORAL_PRIVATE_INCLUDES_VULKAN
    ${CORAL_SRC_DIR}/Vulkan/VulkanHeader.h
)

# Definition of all source files of the Coral library
set(CORAL_SOURCES
    ${CORAL_SRC_DIR}/Buffer.cpp
    ${CORAL_SRC_DIR}/BufferView.cpp
    ${CORAL_SRC_DIR}/CommandBuffer.cpp
    ${CORAL_SRC_DIR}/CommandQueue.cpp
    ${CORAL_SRC_DIR}/DescriptorSet.cpp
    ${CORAL_SRC_DIR}/Context.cpp
    ${CORAL_SRC_DIR}/Fence.cpp
    ${CORAL_SRC_DIR}/PipelineState.cpp
    ${CORAL_SRC_DIR}/RAII.cpp
    ${CORAL_SRC_DIR}/Framebuffer.cpp
    ${CORAL_SRC_DIR}/Semaphore.cpp
    ${CORAL_SRC_DIR}/Sampler.cpp
    ${CORAL_SRC_DIR}/ShaderModule.cpp
    ${CORAL_SRC_DIR}/Surface.cpp
    ${CORAL_SRC_DIR}/Image.cpp
    ${CORAL_SRC_DIR}/UniformBlockBuilder.cpp
)

set(CORAL_SOURCES_VULKAN
    ${CORAL_SRC_DIR}/Vulkan/Buffer.cpp
    ${CORAL_SRC_DIR}/Vulkan/BufferView.cpp
    ${CORAL_SRC_DIR}/Vulkan/CommandBuffer.cpp
    ${CORAL_SRC_DIR}/Vulkan/CommandQueue.cpp
    ${CORAL_SRC_DIR}/Vulkan/DescriptorSet.cpp
    ${CORAL_SRC_DIR}/Vulkan/Context.cpp
    ${CORAL_SRC_DIR}/Vulkan/Fence.cpp
    ${CORAL_SRC_DIR}/Vulkan/Image.cpp
    ${CORAL_SRC_DIR}/Vulkan/PipelineState.cpp
    ${CORAL_SRC_DIR}/Vulkan/Framebuffer.cpp
    ${CORAL_SRC_DIR}/Vulkan/Semaphore.cpp
    ${CORAL_SRC_DIR}/Vulkan/Sampler.cpp
    ${CORAL_SRC_DIR}/Vulkan/ShaderModule.cpp
    ${CORAL_SRC_DIR}/Vulkan/SurfaceWin32.cpp
    ${CORAL_SRC_DIR}/Vulkan/Surface.cpp
    ${CORAL_SRC_DIR}/Vulkan/BufferPool.cpp
)

###############################################################################
# Link the Coral library with the following libraries
###############################################################################

find_package(VulkanHeaders REQUIRED)
find_package(volk CONFIG REQUIRED)
find_package(vk-bootstrap REQUIRED)
find_package(vulkan-memory-allocator REQUIRED)

###############################################################################
# Create the Coral library
###############################################################################

AddTarget(TARGET_NAME ${CORAL_TARGET_NAME}
          TARGET_TYPE ${CORAL_TARGET_TYPE}
          PUBLIC_MODULES ${CORAL_PUBLIC_MODULES}
          PRIVATE_MODULES ${CORAL_PRIVATE_MODULES} ${CORAL_PRIVATE_MODULES_VULKAN}
          MODULE_SOURCES ${CORAL_SOURCES} ${CORAL_SOURCES_VULKAN}
          PRIVATE_HEADERS
            ${CORAL_PRIVATE_INCLUDES_VULKAN}
          PRIVATE_DEPENDENCIES 
              Vulkan::Headers
              volk::volk
              vulkan-memory-allocator::vulkan-memory-allocator
              vk-bootstrap::vk-bootstrap
              spirv-reflect-static
              Common)

###############################################################################
# Organize files in the IDE
###############################################################################

source_group("include/public/${CORAL_TARGET_NAME}" FILES ${CORAL_PUBLIC_MODULES})
source_group("include/private/${CORAL_TARGET_NAME}" FILES ${CORAL_PRIVATE_MODULES})
source_group("include/private/${CORAL_TARGET_NAME}/Vulkan" FILES ${CORAL_PRIVATE_MODULES_VULKAN} ${CORAL_PRIVATE_INCLUDES_VULKAN})
source_group("src/${CORAL_TARGET_NAME}" FILES ${CORAL_SOURCES})
source_group("src/${CORAL_TARGET_NAME}/Vulkan" FILES ${CORAL_SOURCES_VULKAN})
