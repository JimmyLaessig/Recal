module;

#include <cassert>
#include <cmath>
#include <vector>
#include <memory>

module Engine.Image;

namespace
{

uint32_t
getAlignmentBytes(Reef::Image::Alignment alignment)
{
	switch (alignment)
	{
		case Reef::Image::Alignment::BYTE:			return 1;
		case Reef::Image::Alignment::TWO_BYTES:		return 2;
		case Reef::Image::Alignment::FOUR_BYTES:	return 4;
		case Reef::Image::Alignment::EIGHT_BYTES:	return 8;
	}

	assert(false);
	return 1;
}


uint32_t
getNextMultipleOf(uint32_t value, uint32_t alignment)
{
	auto remainder = value % alignment;

	if (remainder == 0)
	{
		return value;
	}

	return value + alignment - remainder;
}

} // namespace


namespace Reef
{
class Impl
{
public:

	Impl(uint32_t width, uint32_t height, Image::PixelFormat pixelFormat)
		: Impl(width, height, pixelFormat, Image::Alignment::FOUR_BYTES)
	{
	}

	Impl(uint32_t width, uint32_t height, Image::PixelFormat pixelFormat, Image::Alignment alignment)
		: Impl(width, height, pixelFormat, alignment, nullptr)
	{
	}

	Impl(uint32_t width, uint32_t height, Image::PixelFormat pixelFormat, Image::Alignment alignment, void* data)
		: mWidth(width)
		, mHeight(height)
		, mPixelFormat(pixelFormat)
		, mAlignment(alignment)
	{
		if (data != nullptr)
		{
			size_t stride = rowStride();
			size_t bufferSize = stride * static_cast<size_t>(mHeight);
			mData = std::vector<std::byte>((const std::byte*)data, (const std::byte*)data + bufferSize);
		}
	}

	uint32_t width() const { return mWidth; }

	uint32_t height() const { return mHeight; }

	uint32_t rowStride() const { return getNextMultipleOf(mWidth * Reef::sizeInBytes(mPixelFormat), getAlignmentBytes(mAlignment)); }

	Image::Alignment alignment() const { return mAlignment; }

	Image::PixelFormat pixelFormat() const { return mPixelFormat; }

	const void* data() const { return mData.data(); }

	const void* at(uint32_t x, uint32_t y) const
	{
		return mData.data() + y * rowStride() + x * sizeInBytes(mPixelFormat);
	}

	void* at(uint32_t x, uint32_t y)
	{
		return mData.data() + y * rowStride() + x * sizeInBytes(mPixelFormat);
	}

private:

	uint32_t mWidth{ 0 };

	uint32_t mHeight{ 0 };

	Image::Alignment mAlignment{ Image::Alignment::FOUR_BYTES };

	Image::PixelFormat mPixelFormat{ Image::PixelFormat::RGBA32 };

	std::vector<std::byte> mData;
};


Image::Image()
	: mImpl(std::make_unique<Impl>(0, 0, Reef::Image::PixelFormat::RGBA32))
{
}


Image::Image(uint32_t width, uint32_t height, PixelFormat format)
	: mImpl(std::make_unique<Impl>(width, height, format))
{}


Image::Image(uint32_t width, uint32_t height, PixelFormat format, Alignment alignment)
	: mImpl(std::make_unique<Impl>(width, height, format, alignment))
{}


Image::Image(uint32_t width, uint32_t height, PixelFormat format, Alignment alignment, void* data)
	: mImpl(std::make_unique<Impl>(width, height, format, alignment, data))
{}


Image::~Image()
{

}


Image::Image(const Image& other)
{
	mImpl = std::make_unique<Impl>(*other.mImpl);
}


Image&
Image::operator=(const Image& other)
{
	mImpl = std::make_unique<Impl>(*other.mImpl);
	return *this;
}


uint32_t
Image::width() const 
{ 
	return mImpl->width();
}


uint32_t
Image::height() const
{ 
	return mImpl->height();
}


uint32_t 
Image::rowStride() const
{
	return mImpl->rowStride();
}


Image::Alignment
Image::alignment() const
{
	return mImpl->alignment();
}


Image::PixelFormat
Image::pixelFormat() const
{
	return mImpl->pixelFormat();
}


void
Image::reset(uint32_t width, uint32_t height, PixelFormat format, Alignment alignment, void* data)
{
	mImpl = std::make_unique<Impl>(width, height, format, alignment, data);
}


const void*
Image::data() const
{ 
	return mImpl->data();
}


const void* 
Image::at(uint32_t x, uint32_t y) const
{
	return mImpl->at(x, y);
}


void* 
Image::at(uint32_t x, uint32_t y)
{
	return mImpl->at(x, y);
}


const void* 
Image::operator()(uint32_t x, uint32_t y) const
{
	return at(x, y);
}


void* 
Image::operator()(uint32_t x, uint32_t y)
{
	return at(x, y);
}

} // namespace Base