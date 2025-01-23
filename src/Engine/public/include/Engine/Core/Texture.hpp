#ifndef ENGINE_CORE_TEXTURE_HPP
#define ENGINE_CORE_TEXTURE_HPP

#include <Engine/Core/Resource.hpp>
#include <Engine/Core/TextureFormat.hpp>

#include <cassert>
#include <memory>
#include <span>
#include <string>
#include <string_view>
#include <vector>


namespace Reef
{

enum class Tiling
{
	Wrap,
	Clamp,
	Mirror,
	ClampToOne,
	ClampToZero,
};


enum class Filter
{
	Nearest,
	Bilinear,
	Trilinar,
};


class ENGINE_API Texture : public SubclassOf<Texture, Resource>
{
public:

	Texture() = default;

	Texture(uint32_t width, uint32_t height, TextureFormat format, std::span<const std::byte> data, std::string_view name = "");

	/// 1x1 RGBA32 texture with a white pixel
	static std::shared_ptr<Texture> White;

	/// 1x1 RGBA32 texture with a black pixel
	static std::shared_ptr<Texture> Black;

	/// 1x1 RGB24 texture with a surface normal vector at a neutral position
	static std::shared_ptr<Texture> Normal;

	bool set(uint32_t width, uint32_t height, TextureFormat format, std::span<const std::byte> data = {});

	TextureFormat format() const { return mTextureFormat; }

	uint32_t width() const { return mWidth; }

	uint32_t height() const { return mHeight; }

	std::span<const std::byte> pixelData() const { return mPixelData; }

	Tiling tiling() const { return mTiling; }

	void setTiling(Tiling tiling) { mTiling = tiling; }

	Filter filter() const { return mFilter; }

	void setFilter(Filter filter) { mFilter = filter; }

	bool hasMipmaps() const { return mHasMips; }

	void setMipmaps(bool hasMipmaps) { mHasMips = hasMipmaps; }

private:

	TextureFormat mTextureFormat{ TextureFormat::RGBA_8U };

	uint32_t mWidth{ 0 };

	uint32_t mHeight{ 0 };

	std::vector<std::byte> mPixelData;

	Tiling mTiling{ Tiling::Wrap };

	Filter mFilter{ Filter::Trilinar };

	bool mHasMips{ true };
};

} // namespace Reef

#endif // !ENGINE_CORE_TEXTURE_HPP