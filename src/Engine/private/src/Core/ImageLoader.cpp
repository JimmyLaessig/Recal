module;

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <cstdint>

module Engine.ImageLoader;


using namespace Reef;

namespace
{

Image::PixelFormat 
getPixelFormat(uint32_t numBytesPerChannel, uint32_t numChannels)
{
	switch (numBytesPerChannel)
	{
		case 1:
			switch (numChannels)
			{
				case 1:	return Image::PixelFormat::GREY8;
				case 2:	return Image::PixelFormat::RG16;
				case 3:	return Image::PixelFormat::RGB24;
				case 4:	return Image::PixelFormat::RGBA32;
				default:
					break;
			}
		case 2:
			switch (numChannels)
			{
				case 1:	return Image::PixelFormat::GREY16;
				case 2:	return Image::PixelFormat::RG32;
				case 3:	return Image::PixelFormat::RGB48;
				case 4:	return Image::PixelFormat::RGBA64;
				default:
					break;
			}
	}

	assert(false);
	return Image::PixelFormat::RGBA32;
}

}


bool ImageLoader::loadImage(const char* path, Image& image)
{
	int x, y, c;
	auto data = stbi_load(path, &x, &y, &c, 0);
	
	if (data == nullptr)
	{
		return false;
	}

	auto pixelFormat = getPixelFormat(1, c);

	image.reset(x, y, pixelFormat, Image::Alignment::BYTE, data);

	stbi_image_free(data);

	return true;
}