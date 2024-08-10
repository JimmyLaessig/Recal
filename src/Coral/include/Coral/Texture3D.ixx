module;

#include <Coral/System.h>

#include <cstdint>

export module Coral.Texture3D;

import Coral.Texture;

export namespace Coral
{

struct Texture3DConfig
{
	/// The Pixel Format of the texture
	PixelFormat format{ PixelFormat::RGBA8_UI };
	
	/// The width of the texture (must be greater than 0)
	uint32_t width{ 1 };
	
	/// The height of the texture (must be greater than 0)
	uint32_t height{ 1 };
	
	/// The depth of the texture (must be greater than 0)
	uint32_t depth{ 1 };
	
	/// Flag indicating whether to generate mip maps
	bool generateMips{ true };
	
	/// The alignment of the each pixel row of the texture data
	Alignment alignment{ Alignment::BYTE };
	
	/// The data to pass into the texture (optional))
	const void* data{ nullptr };
};


class CORAL_API Texture3D : public Texture
{
public:

	virtual uint32_t depth() const = 0;

};


CORAL_API Texture3D* createTexture3D(Context* context, const Texture3DConfig& config);

CORAL_API void destroy(Texture3D* texture);

} // namespace Coral