module;

#include <Coral/System.h>

#include <cstdint>

export module Coral.Texture2D;

import Coral.Texture;

export namespace Coral
{

struct Texture2DConfig
{
	/// The Pixel Format of the texture
	PixelFormat format{ PixelFormat::RGBA8_UI };
	/// The width of the texture (must be greater than 0)
	uint32_t width{ 1 };
	/// The height of the texture (must be greater than 0)
	uint32_t height{ 1 };
	/// Flag indicating whether to generate mip maps
	bool generateMips{ true };
	/// The alignment of the each pixel row of the texture data
	Alignment alignment{ Alignment::FOUR_BYTES };
	/// The data to pass into the texture (optional)
	const void* data{ nullptr };
};


class CORAL_API Texture2D : public Texture
{
public:

};

CORAL_API Texture2D* createTexture2D(Context* context, const Texture2DConfig& config);

CORAL_API void destroy(Texture2D* texture);

} // namespace Coral
