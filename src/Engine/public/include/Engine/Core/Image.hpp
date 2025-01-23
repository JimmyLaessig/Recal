module;

#include <Engine/System.hpp>

#include <cassert>

#include <string>
#include <memory>

export module Engine.Image;


export namespace Reef
{

class ENGINE_API Image
{
public:

	/// The alignment requirements for the start of each pixel row in memory
	/**
	 * Each pixel row must be a multiple of the alignment value
	 */
	enum class Alignment
	{
		BYTE,			// 1-byte alignment (no padding)
		TWO_BYTES,		// Rows aligned to even-numbered bytes (maximum 1 byte padding)
		FOUR_BYTES,		// Rows aligned to 4 bytes (maximum 3 bytes padding)
		EIGHT_BYTES		// Rows aligned to 8 bytes (maximum 7 bytes padding
	};

	enum class PixelFormat
	{
		GREY8,
		RG16,
		RGB24,
		RGBA32,
		GREY16,
		RG32,
		RGB48,
		RGBA64,
	};

	Image();

	Image(uint32_t width, uint32_t height, PixelFormat format);

	Image(uint32_t width, uint32_t height, PixelFormat format, Alignment alignment);

	Image(uint32_t width, uint32_t height, PixelFormat format, Alignment, void* data);

	Image(const Image& other);

	~Image();

	Image& operator=(const Image& other);

	/// Reset the image
	void reset(uint32_t width, uint32_t height, PixelFormat format, Alignment alignment, void* data);

	/// The width of the image (in pixel)
	uint32_t width() const;

	/// The height of the image (in pixel)
	uint32_t height() const;

	/// The stride of one row (in bytes)
	uint32_t rowStride() const;

	/// The alignment of one row
	Alignment alignment() const;

	/// The pixel format of the image
	PixelFormat pixelFormat() const;

	/// The buffer pointing to the first pixel in the image
	const void* data() const;

	/// Get the pointer to the pixel at the position (x, y)
	const void* at(uint32_t x, uint32_t y) const;

	/// Get the pointer to the pixel at the position (x, y)
	void* at(uint32_t x, uint32_t y);

	/// Get the pointer to the pixel at the position (x, y)
	const void* operator()(uint32_t x, uint32_t y) const;

	/// Get the pointer to the pixel at the position (x, y)
	void* operator()(uint32_t x, uint32_t y);

private:

	std::unique_ptr<class Impl> mImpl;
};


constexpr inline uint32_t sizeInBytes(Reef::Image::PixelFormat format)
{
	switch (format)
	{
		case Reef::Image::PixelFormat::GREY8:	return 1;
		case Reef::Image::PixelFormat::RG16:	return 2;
		case Reef::Image::PixelFormat::RGB24:	return 3;
		case Reef::Image::PixelFormat::RGBA32:	return 4;
		case Reef::Image::PixelFormat::GREY16:	return 2;
		case Reef::Image::PixelFormat::RG32:	return 4;
		case Reef::Image::PixelFormat::RGB48:	return 6;
		case Reef::Image::PixelFormat::RGBA64:	return 8;
	}
	assert(false);
	return 1;
}

} // namespace Reef
