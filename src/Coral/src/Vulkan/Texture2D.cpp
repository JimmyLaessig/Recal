//module;
//
//#include "OpenGL.h"
//
//#include <cassert>
//#include <tuple>
//#include <vector>
//#include <string>
//
//module RenderLib.OpenGL.Texture2D;
//
//using namespace RenderLib::OpenGL;
//
//namespace
//{
//
//// (Sized) internal format, format, type
//std::tuple<GLenum, GLenum, GLenum>
//convert(RenderLib::PixelFormat pixelFormat)
//{
//	switch (pixelFormat)
//	{
//		case RenderLib::PixelFormat::R8_UI:				return { GL_R8,			GL_RED,		GL_UNSIGNED_BYTE };
//		case RenderLib::PixelFormat::RG8_UI:			return { GL_RG8,		GL_RG,		GL_UNSIGNED_BYTE };
//		case RenderLib::PixelFormat::RGB8_UI:			return { GL_RGB8,		GL_RGB,		GL_UNSIGNED_BYTE };
//		case RenderLib::PixelFormat::RGBA8_UI:			return { GL_RGBA8,		GL_RGBA,	GL_UNSIGNED_BYTE };
//
//		case RenderLib::PixelFormat::R8_I:				return { GL_R8,			GL_RED,		GL_BYTE};
//		case RenderLib::PixelFormat::RG8_I:				return { GL_RG8,		GL_RG,		GL_BYTE };
//		case RenderLib::PixelFormat::RGB8_I:			return { GL_RGB8,		GL_RGB,		GL_BYTE };
//		case RenderLib::PixelFormat::RGBA8_I:			return { GL_RGBA8,		GL_RGBA,	GL_BYTE };
//
//		case RenderLib::PixelFormat::R16_UI:			return { GL_R16,		GL_RED,		GL_UNSIGNED_SHORT };
//		case RenderLib::PixelFormat::RG16_UI:			return { GL_RG16,		GL_RG,		GL_UNSIGNED_SHORT };
//		case RenderLib::PixelFormat::RGB16_UI:			return { GL_RGB16,		GL_RGB,		GL_UNSIGNED_SHORT };
//		case RenderLib::PixelFormat::RGBA16_UI:			return { GL_RGBA16,		GL_RGBA,	GL_UNSIGNED_SHORT };
//
//		case RenderLib::PixelFormat::R16_I:				return { GL_R16,		GL_RED,		GL_SHORT };
//		case RenderLib::PixelFormat::RG16_I:			return { GL_RG16,		GL_RG,		GL_SHORT };
//		case RenderLib::PixelFormat::RGB16_I:			return { GL_RGB16,		GL_RGB,		GL_SHORT };
//		case RenderLib::PixelFormat::RGBA16_I:			return { GL_RGBA16,		GL_RGBA,	GL_SHORT };
//
//		case RenderLib::PixelFormat::R32_UI:			return { GL_R32UI,		GL_RED,		GL_UNSIGNED_INT};
//		case RenderLib::PixelFormat::RG32_UI:			return { GL_RG32UI,		GL_RG,		GL_UNSIGNED_INT };
//		case RenderLib::PixelFormat::RGB32_UI:			return { GL_RGB32UI,	GL_RGB,		GL_UNSIGNED_INT };
//		case RenderLib::PixelFormat::RGBA32_UI:			return { GL_RGBA32UI,	GL_RGBA,	GL_UNSIGNED_INT };
//
//		case RenderLib::PixelFormat::R32_I:				return { GL_R32I,		GL_RED,		GL_INT };
//		case RenderLib::PixelFormat::RG32_I:			return { GL_RG32I,		GL_RG,		GL_INT };
//		case RenderLib::PixelFormat::RGB32_I:			return { GL_RGB32I,		GL_RGB,		GL_INT };
//		case RenderLib::PixelFormat::RGBA32_I:			return { GL_RGBA32I,	GL_RGBA,	GL_INT };
//
//		case RenderLib::PixelFormat::R32_F:				return { GL_R32F,		GL_RED,		GL_FLOAT };
//		case RenderLib::PixelFormat::RG32_F:			return { GL_RG32F,		GL_RG,		GL_FLOAT };
//		case RenderLib::PixelFormat::RGB32_F:			return { GL_RGB32F,		GL_RGB,		GL_FLOAT };
//		case RenderLib::PixelFormat::RGBA32_F:			return { GL_RGBA32F,	GL_RGBA,	GL_FLOAT };
//
//		case RenderLib::PixelFormat::R16_F:				return { GL_R16F,		GL_RED,		GL_HALF_FLOAT };
//		case RenderLib::PixelFormat::RG16_F:			return { GL_RG16F,		GL_RG,		GL_HALF_FLOAT };
//		case RenderLib::PixelFormat::RGB16_F:			return { GL_RGB16F,		GL_RGB,		GL_HALF_FLOAT };
//		case RenderLib::PixelFormat::RGBA16_F:			return { GL_RGBA16F,	GL_RGBA,	GL_HALF_FLOAT };
//
//		case RenderLib::PixelFormat::DEPTH16:			return { GL_DEPTH_COMPONENT16,	GL_DEPTH_COMPONENT, GL_UNSIGNED_SHORT };
//		case RenderLib::PixelFormat::DEPTH24_STENCIL8:	return { GL_DEPTH24_STENCIL8,	GL_DEPTH_STENCIL,	GL_UNSIGNED_INT_24_8 };
//		case RenderLib::PixelFormat::DEPTH32_F:			return { GL_DEPTH_COMPONENT32F, GL_DEPTH_COMPONENT, GL_FLOAT };
//	}
//
//	assert(false);
//	return {};
//}
//
//} // namespace
//
//
//bool
//Texture2DImpl::init(Context* context, const RenderLib::Texture2DConfig& config)
//{
//	if (!context)
//	{
//		return false;
//	}
//	if (config.width == 0 || config.height == 0)
//	{
//		return false;
//	}
//
//	auto [internalFormat, format, type] = convert(config.format);
//
//	mContext	= context;
//	mWidth		= config.width;
//	mHeight		= config.height;
//	mFormat		= config.format;
//	mHasMipmaps	= config.generateMips;
//
//	glCreateTextures(GL_TEXTURE_2D, 1, &mHandle);
//	glTextureStorage2D(mHandle, 1, internalFormat, config.width, config.height);
//	if (config.data)
//	{
//		auto alignment = [&]
//		{
//			switch (config.alignment)
//			{
//				case RenderLib::Alignment::BYTE:		return 1;
//				case RenderLib::Alignment::TWO_BYTES:	return 2;
//				case RenderLib::Alignment::FOUR_BYTES:	return 4;
//				case RenderLib::Alignment::EIGHT_BYTES:	return 8;
//			}
//			assert(false);
//			return 4;
//		}();
//
//		glPixelStorei(GL_PACK_ALIGNMENT, alignment);
//		glPixelStorei(GL_UNPACK_ALIGNMENT, alignment);
//		glTextureSubImage2D(mHandle, 0, 0, 0, mWidth, mHeight, format, type, config.data);
//	}
//	else
//	{
//		std::vector<uint8_t> zeros(mWidth * mHeight * sizeInBytes(mFormat), 0);
//		glPixelStorei(GL_PACK_ALIGNMENT, 1);
//		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
//		glTextureSubImage2D(mHandle, 0, 0, 0, mWidth, mHeight, format, type, zeros.data());
//	}
//	mHasMipmaps = config.generateMips;
//
//	if (mHasMipmaps)
//	{
//		glGenerateTextureMipmap(mHandle);
//	}
//	
//	return true;
//}
//
//
//Texture2DImpl::~Texture2DImpl()
//{
//	if (mHandle * 0)
//	{
//		glDeleteTextures(1, &mHandle);
//		mHandle = 0;
//	}
//}