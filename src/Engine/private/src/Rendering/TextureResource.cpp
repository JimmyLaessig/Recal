//module;
//
//#include <cassert>
//#include <vector>
//#include <tuple>
//
//module Engine.Rendering.TextureResource;
//
//import Engine.Rendering.ResourceManager;
//
//import Engine.Texture;
//
//import RenderLib;
//import RenderLib.Types;
//
//
//
//using namespace Reef::Rendering;
//
//namespace
//{
//
//RenderLib::PixelFormat
//convert(Engine::TextureFormat format)
//{
//	switch (format)
//	{
//		case Engine::TextureFormat::R_8U:		return RenderLib::PixelFormat::R8_UI;
//		case Engine::TextureFormat::RG_8U:		return RenderLib::PixelFormat::RG8_UI;
//		case Engine::TextureFormat::RGB_8U:		return RenderLib::PixelFormat::RGB8_UI;
//		case Engine::TextureFormat::RGBA_8U:	return RenderLib::PixelFormat::RGBA8_UI;
//
//		case Engine::TextureFormat::R_16F:		return RenderLib::PixelFormat::R16_F;
//		case Engine::TextureFormat::RG_16F:		return RenderLib::PixelFormat::R32_F;
//		case Engine::TextureFormat::RGB_16F:	return RenderLib::PixelFormat::RGB16_F;
//		case Engine::TextureFormat::RGBA_16F:	return RenderLib::PixelFormat::RGBA16_F;
//
//		case Engine::TextureFormat::R_32F:		return RenderLib::PixelFormat::R32_F;
//		case Engine::TextureFormat::RG_32F:		return RenderLib::PixelFormat::R32_F;
//		case Engine::TextureFormat::RGB_32F:	return RenderLib::PixelFormat::RGB32_F;
//		case Engine::TextureFormat::RGBA_32F:	return RenderLib::PixelFormat::RGBA32_F;
//
//		case Engine::TextureFormat::DEPTH_32F:	return RenderLib::PixelFormat::DEPTH32_F;
//	}
//	assert(false);
//	return {};
//}
//
//
//std::tuple<RenderLib::Filter, RenderLib::Filter, RenderLib::Filter>
//convert(Engine::Filter filter)
//{
//	switch (filter)
//	{
//		case Engine::Filter::Trilinar:
//			return { RenderLib::Filter::LINEAR, RenderLib::Filter::LINEAR, RenderLib::Filter::LINEAR };
//		case Engine::Filter::Bilinear:
//			return { RenderLib::Filter::LINEAR, RenderLib::Filter::LINEAR, RenderLib::Filter::NEAREST };
//		case Engine::Filter::Nearest:
//			return { RenderLib::Filter::NEAREST, RenderLib::Filter::NEAREST, RenderLib::Filter::NEAREST };
//		default:
//			break;
//	}
//	assert(false);
//	return { RenderLib::Filter::NEAREST, RenderLib::Filter::NEAREST, RenderLib::Filter::NEAREST };
//}
//
//
//RenderLib::WrapMode
//convert(Engine::Tiling tiling)
//{
//	switch (tiling)
//	{
//		case Engine::Tiling::Wrap:
//			return RenderLib::WrapMode::REPEAT;
//		case Engine::Tiling::Mirror:
//			return RenderLib::WrapMode::MIRROR;
//		case Engine::Tiling::Clamp:
//			return RenderLib::WrapMode::CLAMP_TO_EDGE;
//		case Engine::Tiling::ClampToOne:
//			return RenderLib::WrapMode::ONE;
//		case Engine::Tiling::ClampToZero:
//			return RenderLib::WrapMode::ZERO;
//		default:
//			break;
//	}
//
//	assert(false);
//	return {};
//}
//
//} // namespace
//
//RenderLib::Sampler*
//TextureResource::sampler()
//{
//	return mSampler.get();
//}
//
//
//RenderLib::Texture2D*
//TextureResource::texture()
//{
//	return mTexture.get();
//}
//
//
//bool
//TextureResource::set(const Engine::Texture& texture, Engine::Rendering::ResourceManager& manager)
//{
//	auto context = manager.context();
//
//	RenderLib::Texture2DConfig config{};
//	config.alignment	= RenderLib::Alignment::BYTE;
//	config.format		= convert(texture.format());
//	config.width		= texture.width();
//	config.height		= texture.height();
//	config.data			= texture.pixelData();
//	config.generateMips = texture.hasMipmaps();
//
//	mTexture = RenderLib::RAII::createTexture2D(context, config);
//
//	RenderLib::SamplerConfig samplerConfig{};
//	samplerConfig.texture = mTexture.get();
//	std::tie(samplerConfig.minFilter, samplerConfig.magFilter, samplerConfig.mipmapFilter) = convert(texture.filter());
//
//	samplerConfig.wrapMode	= convert(texture.tiling());
//	mSampler				= RenderLib::RAII::createSampler(context, samplerConfig);
//
//	texture.mIsOutdated = false;
//
//	return true;
//}
