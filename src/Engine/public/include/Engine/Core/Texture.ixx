module;

#include <cassert>
#include <memory>
#include <span>
#include <string>
#include <string_view>
#include <vector>

export module Engine.Texture;

import Engine.Object;
import Engine.SubclassOf;

export import Engine.TextureFormat;

export namespace Reef
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

class Texture : public SubclassOf<Texture, Object>
{
public:

	const std::string& name() const { return mName; }

	void setName(std::string_view name) { mName.assign(name.begin(), name.end()); }

	/// 1x1 RGBA32 texture with a white pixel
	static std::shared_ptr<Texture> white();

	/// 1x1 RGBA32 texture with a black pixel
	static std::shared_ptr<Texture> black();

	/// 1x1 RGB24 texture with a surface normal vector at a neutral position
	static std::shared_ptr<Texture> normal();

	Texture() = default;

	Texture(uint32_t width, uint32_t height, TextureFormat format, std::span<std::byte> data = {});

	bool set(uint32_t width, uint32_t height, TextureFormat format, std::span<std::byte> data = {});

	TextureFormat format() const { return mTextureFormat; }

	uint32_t width() const { return mWidth; }

	uint32_t height() const { return mHeight; }

	const std::byte* pixelData() const { return mPixelData.data(); }

	Tiling tiling() const { return mTiling; }

	void setTiling(Tiling tiling) { mTiling = tiling; }

	Filter filter() const { return mFilter; }

	void setFilter(Filter filter) { mFilter = filter; }

	bool hasMipmaps() const { return mHasMips; }

	void setMipmaps(bool hasMipmaps) { mHasMips = hasMipmaps; }

	bool isOutdated() const { return mIsOutdated; }

private:

	std::string mName;

	TextureFormat mTextureFormat{ TextureFormat::RGBA_8U };

	uint32_t mWidth{ 0 };

	uint32_t mHeight{ 0 };

	std::vector<std::byte> mPixelData;

	Tiling mTiling{ Tiling::Wrap };

	Filter mFilter{ Filter::Trilinar };

	bool mHasMips{ true };

	mutable bool mIsOutdated{ true };
};

} // namespace Reef