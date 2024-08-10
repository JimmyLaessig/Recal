module;

#include <VulkanHeader.h>

#include <cassert>
#include <mutex>
#include <optional>
#include <vector>

module Coral.Vulkan.CommandBuffer;

import Coral.RAII;
import Coral.Vulkan.Context;
import Coral.Vulkan.CommandQueue;
import Coral.Vulkan.Framebuffer;
import Coral.Vulkan.Image;
import Coral.Vulkan.Buffer;
import Coral.Vulkan.BufferView;
import Coral.Vulkan.PipelineState;
import Coral.Vulkan.DescriptorSet;
import Coral.Types;

using namespace Coral::Vulkan;

namespace
{

std::optional<VkIndexType>
convert(Coral::AttributeFormat format)
{
	switch (format)
	{
		case Coral::AttributeFormat::UINT32:
			return VK_INDEX_TYPE_UINT32;
		case Coral::AttributeFormat::UINT16:
			return VK_INDEX_TYPE_UINT16;
		default:
			return {};
	}
}


VkAttachmentLoadOp
convert(Coral::ClearOp clearOp)
{
	switch (clearOp)
	{
	case Coral::ClearOp::DONT_CARE: return VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	case Coral::ClearOp::LOAD:		return VK_ATTACHMENT_LOAD_OP_LOAD;
	case Coral::ClearOp::CLEAR:		return VK_ATTACHMENT_LOAD_OP_CLEAR;
	default:
		assert(false);
		return VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	}
}


} // namespace

void
CommandBuffer::cmdAddImageBarrier(Coral::Vulkan::Image* image,
								  uint32_t baseMipLevel, 
								  uint32_t levelCount,
								  VkImageLayout oldLayout, 
								  VkImageLayout newLayout, 
								  VkAccessFlagBits srcAccessMask, 
								  VkAccessFlags dstAccessMask,
								  VkPipelineStageFlags srcStageFlags,
								  VkPipelineStageFlags dstStageFlags)
{
	VkImageMemoryBarrier barrier{ VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER };
	barrier.oldLayout						= oldLayout;
	barrier.newLayout						= newLayout;
	barrier.srcQueueFamilyIndex				= mContext->getQueueFamilyIndex();
	barrier.dstQueueFamilyIndex				= mContext->getQueueFamilyIndex();
	barrier.image							= image->getVkImage();
	barrier.subresourceRange.aspectMask		= VK_IMAGE_ASPECT_COLOR_BIT;
	barrier.subresourceRange.baseMipLevel	= baseMipLevel;
	barrier.subresourceRange.levelCount		= levelCount;
	barrier.subresourceRange.layerCount		= 1;
	barrier.subresourceRange.baseArrayLayer = 0;
	barrier.srcAccessMask					= srcAccessMask;
	barrier.dstAccessMask					= dstAccessMask;

	vkCmdPipelineBarrier(mCommandBuffer, srcStageFlags, dstStageFlags, 0, 0, nullptr, 0, nullptr, 1, &barrier);
}


CommandBuffer::~CommandBuffer()
{
	if (mCommandPool && mCommandBuffer != VK_NULL_HANDLE)
	{
		vkFreeCommandBuffers(mContext->getVkDevice(), mCommandPool, 1, &mCommandBuffer);
	}
}


bool
CommandBuffer::init(Coral::Vulkan::Context* context, const Coral::CommandBufferConfig& config)
{
	if (!context)
	{
		return false;
	}

	mContext	= context;
	mName		= config.name;

	auto device		= mContext->getVkDevice();
	mCommandPool	= static_cast<Coral::Vulkan::CommandQueue*>(config.queue)->getVkCommandPool();

	VkCommandBufferAllocateInfo allocInfo{ VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO };
	allocInfo.commandPool			= mCommandPool;
	allocInfo.level					= VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandBufferCount	= 1;
	if (vkAllocateCommandBuffers(device, &allocInfo, &mCommandBuffer) != VK_SUCCESS)
	{
		return false;
	}
	
	return true;
}


bool 
CommandBuffer::begin()
{
	VkCommandBufferBeginInfo info{ VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO };
	
	return vkBeginCommandBuffer(mCommandBuffer, &info) == VK_SUCCESS;
}


bool
CommandBuffer::end()
{
	return vkEndCommandBuffer(mCommandBuffer) == VK_SUCCESS;
}


bool
CommandBuffer::cmdBeginRenderPass(const Coral::BeginRenderPassInfo& info)
{
	auto framebuffer = static_cast<Coral::Vulkan::Framebuffer*>(info.framebuffer);

	const auto& colorAttachments = framebuffer->colorAttachments();
	const auto& depthAttachment = framebuffer->depthAttachment();

	// Dynamic rendering requires manual image layout transition for presentable swapchain images. Before rendering, we
	// must transition the image to a supported layout (VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL). Once rendering is
	// done, the image layout must transition to 'VK_IMAGE_LAYOUT_PRESENT_SRC_KHR'in order to present the swapchain
	// image. Both layout transitions must be executed manually via memory barriers. During cmdBeginRenderPass we
	// recorded all swapchain images and after ending the rendering we insert memory barriers to ensure that the images
	// have a presentable layout.
	std::vector<VkImageMemoryBarrier> imageBarriers;
	for (const auto& colorAttachment : colorAttachments)
	{
		auto image = static_cast<Coral::Vulkan::Image*>(colorAttachment.image);
		if (image->presentable())
		{
			VkImageMemoryBarrier barrier{ VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER };
			barrier.dstAccessMask					= VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
			barrier.oldLayout						= VK_IMAGE_LAYOUT_UNDEFINED;
			barrier.newLayout						= VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
			barrier.image							= image->getVkImage();
			barrier.srcQueueFamilyIndex				= mContext->getQueueFamilyIndex();
			barrier.dstQueueFamilyIndex				= mContext->getQueueFamilyIndex();
			barrier.subresourceRange.aspectMask		= VK_IMAGE_ASPECT_COLOR_BIT;
			barrier.subresourceRange.baseArrayLayer = 0;
			barrier.subresourceRange.layerCount		= 1;
			barrier.subresourceRange.baseMipLevel	= 0;
			barrier.subresourceRange.levelCount		= image->getMipLevels();

			imageBarriers.push_back(barrier);

			mPresentableImagesInUse.push_back(image);
		}
	}

	if (!imageBarriers.empty())
	{
		vkCmdPipelineBarrier(mCommandBuffer,
							 VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,				// srcStageMask
							 VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, // dstStageMask
							 0,
							 0,
							 nullptr,
							 0,
							 nullptr,
							 static_cast<uint32_t>(imageBarriers.size()),
							 imageBarriers.data());
	}


	uint32_t i{ 0 };
	std::vector<VkRenderingAttachmentInfo> attachments;
	for (const auto& colorAttachment : colorAttachments)
	{
		auto image = static_cast<Coral::Vulkan::Image*>(colorAttachment.image);

		VkRenderingAttachmentInfo attachmentInfo{ VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO };
		attachmentInfo.loadOp			= convert(colorAttachment.clearOp);
		attachmentInfo.storeOp			= VK_ATTACHMENT_STORE_OP_STORE;
		attachmentInfo.imageLayout		= VK_IMAGE_LAYOUT_ATTACHMENT_OPTIMAL_KHR;//  image->getVkImageLayout();
		attachmentInfo.imageView		= image->getVkImageView();
		attachmentInfo.resolveMode		= VK_RESOLVE_MODE_NONE;
		attachmentInfo.resolveImageView = VK_NULL_HANDLE;
		
		if (colorAttachments.size() == info.clearColor.size())
		{
			attachmentInfo.clearValue.color.float32[0] = info.clearColor[i].color[0];
			attachmentInfo.clearValue.color.float32[1] = info.clearColor[i].color[1];
			attachmentInfo.clearValue.color.float32[2] = info.clearColor[i].color[2];
			attachmentInfo.clearValue.color.float32[3] = info.clearColor[i].color[3];
		}
		
		attachments.push_back(attachmentInfo);
		i++;
	}

	VkRenderingInfo renderingInfo{ VK_STRUCTURE_TYPE_RENDERING_INFO };
	renderingInfo.colorAttachmentCount		= attachments.size();
	renderingInfo.pColorAttachments			= attachments.data();
	renderingInfo.layerCount				= 1;
	renderingInfo.renderArea.offset.x		= 0;
	renderingInfo.renderArea.offset.y		= 0;
	renderingInfo.renderArea.extent.width	= framebuffer->width();
	renderingInfo.renderArea.extent.height	= framebuffer->height();

	VkRenderingAttachmentInfo depthAttachmentInfo{ VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO };

	if (depthAttachment)
	{
		auto image = static_cast<Coral::Vulkan::Image*>(depthAttachment->image);
		
		depthAttachmentInfo.loadOp				= convert(depthAttachment->clearOp);
		depthAttachmentInfo.storeOp				= VK_ATTACHMENT_STORE_OP_STORE;
		depthAttachmentInfo.imageLayout			= VK_IMAGE_LAYOUT_ATTACHMENT_OPTIMAL;// image->getVkImageLayout();
		depthAttachmentInfo.imageView			= image->getVkImageView();
		depthAttachmentInfo.resolveMode			= VK_RESOLVE_MODE_NONE;
		depthAttachmentInfo.resolveImageView	= VK_NULL_HANDLE;

		if (info.clearDepth)
		{
			depthAttachmentInfo.clearValue.depthStencil.depth	= info.clearDepth->depth;
			depthAttachmentInfo.clearValue.depthStencil.stencil = info.clearDepth->stencil;
		}

		renderingInfo.pDepthAttachment		= &depthAttachmentInfo;
		renderingInfo.pStencilAttachment	= &depthAttachmentInfo;
	}


	vkCmdBeginRendering(mCommandBuffer, &renderingInfo);

	return true;
}


bool
CommandBuffer::cmdEndRenderPass()
{
	vkCmdEndRendering(mCommandBuffer);

	if (!mPresentableImagesInUse.empty())
	{
		// Dynamic rendering requires manual image layout transition for presentable swapchain images. Before
		// rendering, we must transition the image to a supported layout (VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL).
		// Once rendering is done, the image layout must transition to 'VK_IMAGE_LAYOUT_PRESENT_SRC_KHR' in order to
		// present the swapchain image. Both layout transitions must be executed manually via memory barriers. During
		// cmdBeginRenderPass we recorded all swapchain images and after ending the rendering we insert memory barriers
		// to ensure that the images have a presentable layout.

		std::vector<VkImageMemoryBarrier> imageBarriers;
		for (auto image : mPresentableImagesInUse)
		{
			VkImageMemoryBarrier barrier{ VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER };
			barrier.srcAccessMask					= VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
			barrier.oldLayout						= VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
			barrier.newLayout						= VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
			barrier.image							= image->getVkImage();
			barrier.subresourceRange.aspectMask		= VK_IMAGE_ASPECT_COLOR_BIT;
			barrier.subresourceRange.baseArrayLayer = 0;
			barrier.subresourceRange.layerCount		= 1;
			barrier.subresourceRange.baseMipLevel	= 0;
			barrier.subresourceRange.levelCount		= image->getMipLevels();

			imageBarriers.push_back(barrier);
		}
		
		vkCmdPipelineBarrier(mCommandBuffer,
							 VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, // srcStageMask
							 VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT,			// dstStageMask
							 0,
							 0,
							 nullptr,
							 0,
							 nullptr,
							 static_cast<uint32_t>(mPresentableImagesInUse.size()), // imageMemoryBarrierCount
							 imageBarriers.data());

		mPresentableImagesInUse.clear();
	}

	return true;
}


bool 
CommandBuffer::cmdClearImage(Coral::Image* image, const Coral::ClearColor& clearColor)
{
	if (image->presentable())
	{
		return false;
	}

	auto vkImage = static_cast<Coral::Vulkan::Image*>(image);
	
	VkClearColorValue color;
	color.float32[0] = clearColor.color[0];
	color.float32[1] = clearColor.color[1];
	color.float32[2] = clearColor.color[2];
	color.float32[3] = clearColor.color[3];

	std::vector<VkImageSubresourceRange> ranges;

	for (uint32_t i = 0; i < image->getMipLevels(); ++i)
	{
		auto& range				= ranges.emplace_back();
		range.aspectMask		= VK_IMAGE_ASPECT_COLOR_BIT;
		range.baseArrayLayer	= 0;
		range.layerCount		= 1;
		range.baseMipLevel		= i;
		range.levelCount		= image->getMipLevels();
	}

	vkCmdClearColorImage(mCommandBuffer, 
						 vkImage->getVkImage(), 
						 VK_IMAGE_LAYOUT_GENERAL,
						 &color, 
						 static_cast<uint32_t>(ranges.size()), 
						 ranges.data());

	return true;
}


bool
CommandBuffer::cmdCopyBuffer(const CopyBufferInfo& info)
{
	auto source = static_cast<Coral::Vulkan::Buffer*>(info.source);
	auto dest = static_cast<Coral::Vulkan::Buffer*>(info.dest);

	VkBufferCopy bufferCopy;
	bufferCopy.srcOffset	= info.sourceOffset;
	bufferCopy.dstOffset	= info.destOffset;
	bufferCopy.size			= info.size;
	vkCmdCopyBuffer(mCommandBuffer, source->getVkBuffer(), dest->getVkBuffer(), 1, &bufferCopy);

	return true;
}


bool
CommandBuffer::cmdCopyImage(const CopyImageInfo& info)
{
	//auto buffer = static_cast<CO
	//vkCmdCopyBufferToImage(mCommandBuffer, info.source)
	return false;
}


bool
CommandBuffer::cmdBindVertexBuffer(Coral::BufferView* vertexBuffer, uint32_t binding)
{
	auto bufferView = static_cast<Coral::Vulkan::BufferView*>(vertexBuffer);
	auto buffer		= static_cast<Coral::Vulkan::Buffer*>(bufferView->buffer());

	if (buffer->type() != BufferType::VERTEX_BUFFER)
	{
		return false;
	}

	auto vkBuffer = buffer->getVkBuffer();
	VkDeviceSize offset = bufferView->offset();
	VkDeviceSize size	= bufferView->numElements() * bufferView->stride() - offset;
	VkDeviceSize stride = bufferView->stride();

	vkCmdBindVertexBuffers2(mCommandBuffer, binding, 1, &vkBuffer, &offset, &size, &stride);
	return true;
}


bool
CommandBuffer::cmdBindIndexBuffer(Coral::BufferView* indexBuffer)
{
	auto bufferView = static_cast<Coral::Vulkan::BufferView*>(indexBuffer);
	auto buffer = static_cast<Coral::Vulkan::Buffer*>(bufferView->buffer());
		
	if (buffer->type() != BufferType::INDEX_BUFFER)
	{
		// TODO bufer type must be index buffer
		return false;
	}

	VkIndexType indexType{};
	switch (bufferView->attributeFormat())
	{
		case AttributeFormat::UINT16 :
			indexType = VK_INDEX_TYPE_UINT16;
			break;
		case AttributeFormat::UINT32:
			indexType = VK_INDEX_TYPE_UINT32;
			break;
		default:
			//TODO Invalid format
			return false;
	}

	if (bufferView->stride() != sizeInBytes(bufferView->attributeFormat()))
	{
		// TODO: INDEX BUFFER MUST BE TIGHTLY PACKED
		return false;
	}

	vkCmdBindIndexBuffer(mCommandBuffer, buffer->getVkBuffer(), bufferView->offset(), indexType);

	return true;
}


bool
CommandBuffer::cmdBindPipeline(Coral::PipelineState* pipelineState)
{
	mLastBoundPipelineState = static_cast<Coral::Vulkan::PipelineState*>(pipelineState);
	vkCmdBindPipeline(mCommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, mLastBoundPipelineState->getVkPipeline());

	return true;
}


bool
CommandBuffer::cmdDrawIndexed(const DrawIndexInfo& info)
{
	vkCmdDrawIndexed(mCommandBuffer, info.indexCount, 1, info.firstIndex, 0, 0);

	return true;
}


bool
CommandBuffer::cmdSetViewport(const Coral::ViewportInfo& info)
{
	if (info.maxDepth < info.minDepth)
	{
		return false;
	}

	if (info.viewport.width == 0 || info.viewport.height == 0)
	{
		return false;
	}

	VkViewport viewport;
	viewport.x			= info.viewport.x;
	viewport.y			= info.viewport.y;
	viewport.width		= info.viewport.width;
	viewport.height		= info.viewport.height;
	viewport.minDepth	= info.minDepth;
	viewport.maxDepth	= info.maxDepth;

	// Vulkan and OpenGL use different coordinate systems. Per default, the Vulkan coordinate system is y-down. To
	// combat the inverted image, we need to flip the viewport by passing negative height and adjust the y-offset.
	if (info.mode == Coral::ViewportMode::Y_UP)
	{
		viewport.x		= info.viewport.x;
		viewport.y		= viewport.height + viewport.y;
		viewport.width	= info.viewport.width;
		viewport.height = -viewport.height;
	}

	VkRect2D rect;
	rect.extent.width	= info.viewport.width;
	rect.extent.height	= info.viewport.height;
	rect.offset.x		= info.viewport.x;
	rect.offset.y		= info.viewport.y;

	vkCmdSetViewport(mCommandBuffer, 0, 1, &viewport);
	vkCmdSetScissor(mCommandBuffer, 0, 1, &rect);

	return true;
}


bool
CommandBuffer::cmdBindDescriptorSet(Coral::DescriptorSet* descriptorSet, uint32_t index)
{
	if (!mLastBoundPipelineState)
	{
		// TODO LOG ERROR
		return false;
	}

	auto layout = mLastBoundPipelineState->getVkPipelineLayout();

	auto impl				= static_cast<Coral::Vulkan::DescriptorSet*>(descriptorSet);
	auto vkDescriptorSet	= impl->getVkDescriptorSet();
	vkCmdBindDescriptorSets(mCommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, layout, index, 1, &vkDescriptorSet, 0, nullptr);

	return true;
}


bool
CommandBuffer::cmdUpdateBufferData(const Coral::UpdateBufferDataInfo& info)
{
	if (info.offset + info.data.size() > info.buffer->size())
	{
		return false;
	}

	auto buffer = static_cast<Coral::Vulkan::Buffer*>(info.buffer);

	auto stagingBuffer = mContext->requestStagingBuffer(info.data.size());

	auto mapped = stagingBuffer->map();
	std::memcpy(mapped + info.offset, info.data.data(), info.data.size());
	stagingBuffer->unmap();

	VkBufferCopy bufferCopy;
	bufferCopy.srcOffset	= 0;
	bufferCopy.dstOffset	= info.offset;
	bufferCopy.size			= info.data.size();

	vkCmdCopyBuffer(mCommandBuffer, stagingBuffer->getVkBuffer(), buffer->getVkBuffer(), 1, &bufferCopy);

	/*VkBufferMemoryBarrier barrier{};
	barrier.buffer = bufferImpl->getVkBuffer();
	barrier.dstQueueFamilyIndex = mContext->getQueueFamilyIndex();
	barrier.srcQueueFamilyIndex = mContext->getQueueFamilyIndex();*/

	/*VkBufferMemoryBarrier barrier{ VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER };
	barrier.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
	barrier.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	barrier.newLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
	barrier.image = image->getVkImage();
	barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	barrier.subresourceRange.baseArrayLayer = 0;
	barrier.subresourceRange.layerCount = 1;
	barrier.subresourceRange.baseMipLevel = 0;
	barrier.subresourceRange.levelCount = image->getMipLevels();*/

	//imageBarriers.push_back(barrier);

	//vkCmdPipelineBarrier(mCommandBuffer, VK_PIPELINE_STAGE_ALL_COMMANDS_BIT, )
	// Store the temporary staging buffer until the command buffer was executed
	mStagingBuffers.push_back(std::move(stagingBuffer));

	return true;
}


bool
CommandBuffer::cmdUpdateImageData(const Coral::UpdateImageDataInfo& info)
{
	auto image = static_cast<Coral::Vulkan::Image*>(info.image);

	auto stagingBuffer = mContext->requestStagingBuffer(info.data.size());

	auto mapped = stagingBuffer->map();
	std::memcpy(mapped, info.data.data(), info.data.size());
	stagingBuffer->unmap();

	// Before copying data, transition the image layout to be optimal for receiving data. 

	cmdAddImageBarrier(image, 
					   0,
					   image->getMipLevels(), 
					   VK_IMAGE_LAYOUT_UNDEFINED,
					   VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
					   VK_ACCESS_TRANSFER_READ_BIT,
					   VK_ACCESS_TRANSFER_WRITE_BIT,
					   VK_PIPELINE_STAGE_TRANSFER_BIT,
					   VK_PIPELINE_STAGE_TRANSFER_BIT);

	VkBufferImageCopy copy{};
	copy.bufferOffset		= 0;
	copy.bufferRowLength	= 0; // TODO: Support non-tightly packed images
	copy.bufferImageHeight	= 0; // TODO: Support non-tightly packed images
	copy.imageExtent.width	= image->width();
	copy.imageExtent.height	= image->height();
	copy.imageExtent.depth	= 1;
	copy.imageOffset.x		= 0;
	copy.imageOffset.y		= 0;
	copy.imageOffset.z		= 0;
	// Copy the data to the first mip level
	copy.imageSubresource.mipLevel			= 0;
	copy.imageSubresource.layerCount		= 1;
	copy.imageSubresource.baseArrayLayer	= 0;
	copy.imageSubresource.aspectMask		= VK_IMAGE_ASPECT_COLOR_BIT;

	vkCmdCopyBufferToImage(mCommandBuffer, stagingBuffer->getVkBuffer(), image->getVkImage(), VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &copy);

	mStagingBuffers.push_back(std::move(stagingBuffer));

	// After copying the buffer data to the image, we need to transition the layout to be readable from a shader. This
	// transition is only needed if we don't generate mip maps

	if (info.updateMips && image->getMipLevels() > 1)
	{
		for (uint32_t i = 1; i < image->getMipLevels(); ++i)
		{
			// Transition the source image layout to TRANSFER SOURCE 
			cmdAddImageBarrier(image,
							   i - 1,
							   1,
							   VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
							   VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
							   VK_ACCESS_TRANSFER_WRITE_BIT,
							   VK_ACCESS_SHADER_READ_BIT,
							   VK_PIPELINE_STAGE_TRANSFER_BIT,
							   VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT);

			// Blit the src image into the dst image
			VkImageBlit imageBlit{};
			imageBlit.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			imageBlit.srcSubresource.layerCount = 1;
			imageBlit.srcSubresource.mipLevel	= i - 1;
			imageBlit.srcOffsets[1].x			= int32_t(image->width() >> (i - 1));
			imageBlit.srcOffsets[1].y			= int32_t(image->height() >> (i - 1));
			imageBlit.srcOffsets[1].z			= 1;

			imageBlit.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			imageBlit.dstSubresource.layerCount = 1;
			imageBlit.dstSubresource.mipLevel	= i;
			imageBlit.dstOffsets[1].x			= int32_t(image->width() >> i);
			imageBlit.dstOffsets[1].y			= int32_t(image->height() >> i);
			imageBlit.dstOffsets[1].z			= 1;

			vkCmdBlitImage(mCommandBuffer, image->getVkImage(), VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, 
					       image->getVkImage(), VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &imageBlit, VK_FILTER_LINEAR);
		}
	}

	// Finally, transition all mip levels to optimal shader

	cmdAddImageBarrier(image,
					   0,
					   image->getMipLevels(),
					   VK_IMAGE_LAYOUT_UNDEFINED,
					   VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
					   VK_ACCESS_TRANSFER_WRITE_BIT,
					   VK_ACCESS_SHADER_READ_BIT,
					   VK_PIPELINE_STAGE_TRANSFER_BIT,
					   VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT);

	return true;
}


VkCommandBuffer 
CommandBuffer::getVkCommandBuffer()
{
	return mCommandBuffer;
}


std::vector<std::unique_ptr<Coral::Vulkan::Buffer>>
CommandBuffer::getStagingBuffers()
{
	std::vector<std::unique_ptr<Coral::Vulkan::Buffer>> stagingBuffers;
	stagingBuffers.swap(mStagingBuffers);

	return stagingBuffers;
}
