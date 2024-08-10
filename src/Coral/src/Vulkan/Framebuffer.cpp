module;

#include <vulkan/vulkan.h>

#include <vector>

#include <cassert>
#include <optional>

module Coral.Vulkan.Framebuffer;

import Coral.Vulkan.Context;
import Coral.Vulkan.Image;
import Coral.Vulkan.Format;

using namespace Coral::Vulkan;

namespace
{

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


std::optional<VkExtent2D>
getFramebufferExtent(const Coral::FramebufferConfig& config)
{
	if (!config.colorAttachment.empty())
	{
		return VkExtent2D{ config.colorAttachment.front().image->width(), config.colorAttachment.front().image->height() };
	}
	else if (config.depthAttachment)
	{
		return VkExtent2D{ config.depthAttachment->image->width(), config.depthAttachment->image->height() };
	}

	return {};
}

} // namespace

bool
Framebuffer::init(Coral::Vulkan::Context* context, const Coral::FramebufferConfig& config)
{
	mContext = context;

	std::vector<VkAttachmentDescription> attachmentDescriptions;
	attachmentDescriptions.reserve(config.colorAttachment.size());

	std::vector<VkAttachmentReference> attachmentReferences;
	attachmentReferences.reserve(config.colorAttachment.size());

	VkAttachmentReference depthStencilAttachmentRefernce;

	auto extent = getFramebufferExtent(config);

	if (!extent)
	{
		return false;
	}

	mWidth = extent->width;
	mHeight = extent->height;
	
	for (auto attachment : config.colorAttachment)
	{
		mColorAttachments.push_back(attachment);
		/*auto image = static_cast<Coral::Vulkan::Image*>(attachment.image);

		auto& description = attachmentDescriptions.emplace_back();

		description.format			= convert(image->format());
		description.samples			= VK_SAMPLE_COUNT_1_BIT;
		description.loadOp			= ::convert(attachment.clearOp);
		description.storeOp			= VK_ATTACHMENT_STORE_OP_STORE;
		description.stencilLoadOp	= VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		description.stencilStoreOp	= VK_ATTACHMENT_STORE_OP_DONT_CARE;
		description.initialLayout	= VK_IMAGE_LAYOUT_UNDEFINED;
		description.finalLayout		= image->getVkImageLayout();

		auto& reference			= attachmentReferences.emplace_back();
		reference.attachment	= static_cast<uint32_t>(attachmentDescriptions.size() - 1);
		reference.layout		= VK_IMAGE_LAYOUT_GENERAL;*/
	}
	
	
	mDepthAttachment = config.depthAttachment;

	/*if (config.depthAttachment)
	{
		auto image = static_cast<Coral::Vulkan::Image*>(config.depthAttachment->image);

		if (!isDepthFormat(image->format()))
		{
			return false;
		}

		bool stencil = isStencilFormat(image->format());

		auto& description = attachmentDescriptions.emplace_back();

		description.format			= convert(image->format());
		description.samples			= VK_SAMPLE_COUNT_1_BIT;
		description.loadOp			= ::convert(config.depthAttachment->clearOp);
		description.stencilLoadOp	= ::convert(config.depthAttachment->clearOp);
		description.storeOp			= VK_ATTACHMENT_STORE_OP_STORE;
		description.stencilStoreOp	= VK_ATTACHMENT_STORE_OP_STORE;
		description.initialLayout	= VK_IMAGE_LAYOUT_UNDEFINED;
		description.finalLayout		= image->getVkImageLayout();

		depthStencilAttachmentRefernce.attachment = static_cast<uint32_t>(attachmentDescriptions.size() - 1);
		depthStencilAttachmentRefernce.layout = VK_IMAGE_LAYOUT_GENERAL;
	}*/

	//VkSubpassDescription subpass{};
	//subpass.pipelineBindPoint		= VK_PIPELINE_BIND_POINT_GRAPHICS;
	//subpass.colorAttachmentCount	= static_cast<uint32_t>(attachmentReferences.size());
	//subpass.pColorAttachments		= attachmentReferences.data();
	//subpass.pDepthStencilAttachment = config.depthAttachment ? &depthStencilAttachmentRefernce : nullptr;

	//// TODO Handle input from shader
	//subpass.inputAttachmentCount	= 0;
	//subpass.pInputAttachments		= nullptr;

	//VkSubpassDependency dependency{};
	//dependency.srcSubpass		= VK_SUBPASS_EXTERNAL;
	//dependency.dstSubpass		= 0;
	//dependency.srcStageMask		= VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	//dependency.srcAccessMask	= 0;
	//dependency.dstStageMask		= VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	//dependency.dstAccessMask	= VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
	//dependency.dependencyFlags	= 0;

	//VkFramebufferCreateInfo info{ VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO };
	//info.attachmentCount	= static_cast<uint32_t>(attachmentDescriptions.size());
	//info.pAttachments		= attachmentDescriptions.data();
	//info.subpassCount		= 1;
	//info.pSubpasses			= &subpass;
	//info.dependencyCount	= 1;
	//info.pDependencies		= &dependency;
	//
	//if (vkCreateFramebuffer(mContext->getVkDevice(), &info, nullptr, &mFramebuffer) != VK_SUCCESS)
	//{
	//	return false;
	//}
	//
	//std::vector<VkImageView> attachments;

	//for (auto attachment : config.colorAttachment)
	//{
	//	auto imageView = static_cast<Coral::Vulkan::Image*>(attachment.image)->getVkImageView();
	//	attachments.push_back(imageView);
	//}

	//if (config.depthAttachment)
	//{
	//	auto imageView = static_cast<Coral::Vulkan::Image*>(config.depthAttachment->image)->getVkImageView();
	//	attachments.push_back(imageView);
	//}

	//VkFramebufferCreateInfo createInfo{ VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO };
	//createInfo.renderPass		= mFramebuffer;
	//createInfo.width			= mWidth;
	//createInfo.height			= mHeight;
	//createInfo.attachmentCount	= static_cast<uint32_t>(attachments.size());
	//createInfo.pAttachments		= attachments.data();
	//createInfo.layers			= 1;



	return true;
}


const std::vector<Coral::ColorAttachment>&
Framebuffer::colorAttachments()
{
	return mColorAttachments;
}

const std::optional<Coral::DepthAttachment>&
Framebuffer::depthAttachment()
{
	return mDepthAttachment;
}


uint32_t
Framebuffer::width()
{
	return mWidth;
}


uint32_t
Framebuffer::height()
{
	return mHeight;
}


Coral::FramebufferSignature
Framebuffer::getSignature()
{
	Coral::FramebufferSignature signature{};

	if (mDepthAttachment)
	{
		signature.depthStencilAttachmentFormat = mDepthAttachment->image->format();
	}

	for (const auto& colorAttachment : mColorAttachments)
	{
		signature.colorAttachmentFormats.push_back(colorAttachment.image->format());
	}

	return signature;
}