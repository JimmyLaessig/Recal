module;

#include <VulkanHeader.h>

#include <memory>

export module Coral.Vulkan.Surface;

export import Coral.Surface;

import Coral.Vulkan.Semaphore;
import Coral.Vulkan.Image;
import Coral.Vulkan.Framebuffer;

namespace Coral
{

namespace Vulkan
{

export class Context;

export class Surface : public Coral::Surface
{
public:

	virtual ~Surface();

	bool init(Coral::Vulkan::Context* context, const Coral::SurfaceConfig& config);

	VkSurfaceKHR getVkSurface();

	VkSwapchainKHR getVkSwapchain();

	void* nativeWindowHandle() override;

	bool acquireNextSwapchainImage(Coral::Fence* fence) override;

	uint32_t getCurrentSwapchainImageIndex() override;

	Coral::Image* getSwapchainImage(uint32_t index) override;

	Coral::Image* getSwapchainDepthImage() override;

	Coral::Framebuffer* getSwapchainFramebuffer(uint32_t index) override;

private:

	bool initSwapchain(const Coral::SwapchainConfig& config);

	Context* mContext{ nullptr };

	void* mNativeWindowHandle{ nullptr };

	VkSurfaceKHR mSurface{ VK_NULL_HANDLE };

	VkSwapchainKHR mSwapchain{ VK_NULL_HANDLE };

	uint32_t mSwapchainImageCount{ 0 };

	uint32_t mCurrentSwapchainIndex{ 0 };

	struct SwapchainImageData
	{
		std::unique_ptr<Image> image;
		std::unique_ptr<Framebuffer> framebuffer;
	};

	VkSemaphore mSemaphore{ VK_NULL_HANDLE };

	std::vector<SwapchainImageData> mSwapchainImageData;
	std::unique_ptr<Image> mSwapchainDepthImage;

	Coral::SwapchainConfig mConfig{};
}; // class Surface

} // namespace Vulkan

} // namespace Coral