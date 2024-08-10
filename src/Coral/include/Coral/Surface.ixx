module;

#include <Coral/System.h>

#include <cstdint>

#include <optional>

export module Coral.Surface;

import Coral.Types;


export namespace Coral
{

class Context;
class Semaphore;
class Fence;
class Image;
class Framebuffer;

struct CORAL_API SwapchainConfig
{
	PixelFormat format{ Coral::PixelFormat::RGBA8_SRGB };

	std::optional<PixelFormat> depthFormat{ Coral::PixelFormat::DEPTH24_STENCIL8 };

	uint32_t swapchainImageCount{ 3 };
};

struct CORAL_API SurfaceConfig
{
	SwapchainConfig swapchainConfig{};

	void* nativeWindowHandle{ nullptr };
};


class CORAL_API Surface
{

public:
	
	virtual ~Surface() = default;

	virtual void* nativeWindowHandle() = 0;

	virtual bool acquireNextSwapchainImage(Fence* fence) = 0;

	//virtual Semaphore& getImageAvailableSemaphore() = 0;

	virtual uint32_t getCurrentSwapchainImageIndex() = 0;

	virtual Image* getSwapchainImage(uint32_t index) = 0;

	virtual Image* getSwapchainDepthImage() = 0;

	virtual Framebuffer* getSwapchainFramebuffer(uint32_t index) = 0;

};

// Create a new Surface object
CORAL_API Surface* createSurface(Context* context, const SurfaceConfig& config);

// Destroy the Surface object
CORAL_API void destroy(Surface* surface);

} // namespace Coral