module;

#include <VulkanHeader.h>

export module Coral.Vulkan.SurfaceWin32;

namespace Coral
{

namespace Vulkan
{

export VkSurfaceKHR createVkSurface(VkInstance instance, void* hwnd);

} // namespace GL

} // namespace Coral