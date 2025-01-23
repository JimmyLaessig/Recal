#include <Engine/Rendering/TextureResource.hpp>

#include <glm/glm.hpp>

#include <cassert>
#include <vector>
#include <memory>
#include <span>

using namespace Reef::Rendering;

namespace
{

Coral::PixelFormat
convert(Reef::TextureFormat format)
{
	switch (format)
	{
		case Reef::TextureFormat::R_8U:		 return Coral::PixelFormat::R8_UI;
		case Reef::TextureFormat::RG_8U:	 return Coral::PixelFormat::RG8_UI;
		case Reef::TextureFormat::RGB_8U:	 return Coral::PixelFormat::RGB8_UI;
		case Reef::TextureFormat::RGBA_8U:	 return Coral::PixelFormat::RGBA8_UI;
		case Reef::TextureFormat::R_16F:	 return Coral::PixelFormat::R16_F;
		case Reef::TextureFormat::RG_16F:	 return Coral::PixelFormat::RG16_F;
		case Reef::TextureFormat::RGB_16F:	 return Coral::PixelFormat::RGB16_F;
		case Reef::TextureFormat::RGBA_16F:	 return Coral::PixelFormat::RGBA16_F;
		case Reef::TextureFormat::R_32F:	 return Coral::PixelFormat::R32_F;
		case Reef::TextureFormat::RG_32F:	 return Coral::PixelFormat::RG32_F;
		case Reef::TextureFormat::RGB_32F:	 return Coral::PixelFormat::RGB32_F;
		case Reef::TextureFormat::RGBA_32F:	 return Coral::PixelFormat::RGBA32_F;
		case Reef::TextureFormat::DEPTH_32F: return Coral::PixelFormat::DEPTH32_F;
	}

	assert(false);
	return Coral::PixelFormat::RGBA8_UI;
}

} // namespace


TextureResource::TextureResource(ResourceManager& resourceManager)
	: RenderResource(resourceManager)
{}


Coral::Sampler*
TextureResource::sampler()
{
	return mSampler.get();
}


Coral::Image*
TextureResource::texture()
{
	return mTexture.get();
}


bool
TextureResource::update(const Reef::Texture& texture, Coral::Context& context)
{
	if (!shouldUpdate(texture.version()))
	{
		return true;
	}

	Coral::ImageConfig imageConfig{};
	imageConfig.format  = convert(texture.format());
	imageConfig.width   = texture.width();
	imageConfig.height  = texture.height();
	imageConfig.hasMips = texture.hasMipmaps();
	mTexture = Coral::RAII::createImage(&context, imageConfig);
	
	if (!mTexture)
	{
		return false;
	}

	Coral::SamplerConfig samplerConfig{};
	samplerConfig.image = mTexture.get();
	mSampler = Coral::RAII::createSampler(&context, samplerConfig);

	if (!mSampler)
	{
		return false;
	}

	auto queue = context.getTransferQueue();

	Coral::CommandBufferConfig commandBufferConfig{};
	commandBufferConfig.queue = queue;
	commandBufferConfig.name = "Update texture resource";

	auto commandBuffer = Coral::RAII::createCommandBuffer(&context, commandBufferConfig);
	auto comamndBufferPtr = commandBuffer.get();

	commandBuffer->begin();

	Coral::UpdateImageDataInfo info{};
	info.data		= texture.pixelData();
	info.updateMips = texture.hasMipmaps();
	info.image		= mTexture.get();

	commandBuffer->cmdUpdateImageData(info);

	commandBuffer->end();
	auto semaphore		= Coral::RAII::createSemaphore(&context);
	auto semaphorePtr	= semaphore.get();
	Coral::CommandBufferSubmitInfo submitInfo{};

	submitInfo.commandBuffers	= { &comamndBufferPtr , 1 };
	submitInfo.signalSemaphores = { &semaphorePtr , 1 };
	queue->submit(submitInfo, nullptr);

	semaphore->wait();

	return true;
}
