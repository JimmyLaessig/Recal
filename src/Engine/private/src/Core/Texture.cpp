#include <Engine/Core/Texture.hpp>

#include <array>
#include <span>
#include <cstddef>
#include <memory>

using namespace Reef;


/// 1x1 RGBA32 texture with a white pixel
std::shared_ptr<Texture> Texture::White = std::make_shared<Texture>(1u, 1u, TextureFormat::RGBA_8U, std::array<std::byte, 4>{ (std::byte)255u, (std::byte)255u, (std::byte)255u, (std::byte)255u }, "1x1 White");

/// 1x1 RGBA32 texture with a black pixel
std::shared_ptr<Texture> Texture::Black = std::make_shared<Texture>(1u, 1u, TextureFormat::RGBA_8U, std::array<std::byte, 4>{ (std::byte)0u, (std::byte)0u, (std::byte)0u, (std::byte)0u }, "1x1 Black");

/// 1x1 RGB24 texture with a surface normal vector at a neutral position
std::shared_ptr<Texture> Texture::Normal = std::make_shared<Texture>(1u, 1u, TextureFormat::RGBA_8U, std::array<std::byte, 3>{ (std::byte)128, (std::byte)128, (std::byte)255 }, "1x1 Normal");

bool
Texture::set(uint32_t width, uint32_t height, TextureFormat format, std::span<const std::byte> data)
{
	mWidth = width;
	mHeight = height;
	mTextureFormat = format;
	mPixelData.assign(data.begin(), data.end());

	markOutdated();

	return true;
}


Texture::Texture(uint32_t width, uint32_t height, TextureFormat format, std::span<const std::byte> data, std::string_view name)
{
	setName(name);
	set(width, height, format, data);
}