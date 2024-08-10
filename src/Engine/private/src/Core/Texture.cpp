module;

#include <array>
#include <span>
#include <cstddef>
#include <memory>

module Engine.Texture;



using namespace Reef;


std::shared_ptr<Texture>
Texture::white()
{
	static std::shared_ptr<Texture> texture;

	if (!texture)
	{
		std::array<std::byte, 4> white = { (std::byte)255u, (std::byte)255u, (std::byte)255u, (std::byte)255u };
		std::span span = white;
		texture = std::make_shared<Texture>(1u, 1u, TextureFormat::RGBA_8U, span);
		texture->setName("1x1White");
	}

	return texture;
}


std::shared_ptr<Texture>
Texture::black()
{
	static std::shared_ptr<Texture> texture;

	if (!texture)
	{
		std::array<std::byte, 4> black = { (std::byte)0, (std::byte)0, (std::byte)0, (std::byte)0 };
		std::span span = black;
		texture = std::make_shared<Texture>(1u, 1u, TextureFormat::RGBA_8U, span);
		texture->setName("1x1Black");
	}

	return texture;
}


std::shared_ptr<Texture>
Texture::normal()
{
	static std::shared_ptr<Texture> texture;

	if (!texture)
	{
		std::array<std::byte, 3> normal = { (std::byte)128, (std::byte)128, (std::byte)255 };
		std::span span = normal;
		texture = std::make_shared<Texture>(1u, 1u, TextureFormat::RGB_8U, span);
		texture->setName("1x1Normal");
	}

	return texture;
}


Texture::Texture(uint32_t width, uint32_t height, TextureFormat format, std::span<std::byte> data)
{
	set(width, height, format, data);
}


bool 
Texture::set(uint32_t width, uint32_t height, TextureFormat format, std::span<std::byte> data)
{
	mWidth = width;
	mHeight = height;
	mTextureFormat = format;
	mPixelData.assign(data.begin(), data.end());

	mIsOutdated = true;

	return true;
}