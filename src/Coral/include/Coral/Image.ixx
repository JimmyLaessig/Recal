module;

#include <Coral/System.h>

#include <cstdint>
#include <cstddef>

export module Coral.Image;

import Coral.Types;


export namespace Coral
{

enum class Access
{
	
};

export class Context;

/// Configuration to create a Image
struct ImageConfig
{
	/// The width of the image
	uint32_t width{ 0 };

	/// The height of the image
	uint32_t height{ 0 };

	/// Flag indicating if the image should be equipped with multiple levels of detail for minified sampling of the image.
	bool hasMips{ false };

	/// The format and type of the texel blocks contained in the image
	PixelFormat format{ PixelFormat::RGBA8_UI };
};


class Image
{
public:

	virtual ~Image() = default;

	/// The width of the image
	virtual uint32_t width() const = 0;

	/// The height of the image
	virtual uint32_t height() const = 0;

	/// Get the pixel format of the image
	virtual PixelFormat format() const = 0;

	/// Get the number of mipmap levels
	virtual uint32_t getMipLevels() const = 0;

	/// Flag indicating if the image is presentable, e.g. it is part of a swapchain.
	/**
	 * Presentable images have limitations on usage. They cannot be used for:
	 * * Clearing via cmdClearImage.
	 * * 
	 */
	virtual bool presentable() const = 0;
};

/// Create a new buffer view
CORAL_API Image* createImage(Context* context, const ImageConfig& config);

/// Destroy the buffer view
CORAL_API void destroy(Image* image);

} // namespace Coral
