module;

#include <cassert>
#include <cstdint>

export module Engine.TextureFormat;


export namespace Reef
{

enum class TextureFormat
{
	R_8U,
	RG_8U,
	RGB_8U,
	RGBA_8U,

	R_16F,
	RG_16F,
	RGB_16F,
	RGBA_16F,

	R_32F,
	RG_32F,
	RGB_32F,
	RGBA_32F,

	DEPTH_32F,
};


constexpr uint32_t
sizeInBytes(TextureFormat format)
{
	switch (format)
	{
		case TextureFormat::R_8U:		return 1;
		case TextureFormat::RG_8U:		return 2;
		case TextureFormat::RGB_8U:		return 3;
		case TextureFormat::RGBA_8U:	return 4;

		case TextureFormat::R_16F:		return 2;
		case TextureFormat::RG_16F:		return 4;
		case TextureFormat::RGB_16F:	return 6;
		case TextureFormat::RGBA_16F:	return 8;

		case TextureFormat::R_32F:		return 4;
		case TextureFormat::RG_32F:		return 8;
		case TextureFormat::RGB_32F:	return 12;
		case TextureFormat::RGBA_32F:	return 16;

		case TextureFormat::DEPTH_32F:	return 4;
	}

	assert(false);
	return 0;
}

} // namespace Reef