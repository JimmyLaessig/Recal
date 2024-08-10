module;

#include <VulkanHeader.h>

#include <variant>
#include <vector>

module Coral.Vulkan.DescriptorSet;

import Coral.Vulkan.Context;
import Coral.Vulkan.Buffer;
import Coral.Vulkan.Format;
import Coral.Vulkan.Sampler;
import Coral.Vulkan.Image;
import Coral.PipelineState;
import Common.Visitor;

using namespace Coral::Vulkan;


DescriptorSet::~DescriptorSet()
{
	if (mContext)
	{
		if (mDescriptorSet)
		{
			vkFreeDescriptorSets(mContext->getVkDevice(), mContext->getVkDescriptorPool(), 1, &mDescriptorSet);
		}

		if (mDescriptorSetLayout)
		{
			vkDestroyDescriptorSetLayout(mContext->getVkDevice(), mDescriptorSetLayout, nullptr);
		}
	}
}


VkDescriptorSet
DescriptorSet::getVkDescriptorSet()
{
	return mDescriptorSet;
}


bool
DescriptorSet::init(Coral::Vulkan::Context* context, const Coral::DescriptorSetConfig& config)
{
	if (!context)
	{
		return false;
	}

	mContext = context;

	std::vector<VkDescriptorSetLayoutBinding> bindings;

	for (const auto& binding : config.bindings)
	{
		auto& vkBinding				= bindings.emplace_back();
		vkBinding.binding			= binding.binding;
		vkBinding.descriptorCount	= 1;
		vkBinding.stageFlags		= VK_SHADER_STAGE_ALL_GRAPHICS;
		vkBinding.descriptorType	= std::visit(Common::Visitor{
			[](const Coral::Buffer*) -> VkDescriptorType { return VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER; },
			[](const Coral::Sampler*) -> VkDescriptorType { return VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER; }
		}, binding.descriptor);
	}

	VkDescriptorSetLayoutCreateInfo layoutInfo{ VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO };
	layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());
	layoutInfo.pBindings	= bindings.data();

	if (vkCreateDescriptorSetLayout(context->getVkDevice(), &layoutInfo, nullptr, &mDescriptorSetLayout) != VK_SUCCESS)
	{
		return false;
	}
	 
	VkDescriptorSetAllocateInfo info{ VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO };
	info.descriptorPool		= mContext->getVkDescriptorPool();
	info.descriptorSetCount = 1;
	info.pSetLayouts		= &mDescriptorSetLayout;

	if (vkAllocateDescriptorSets(mContext->getVkDevice(), &info, &mDescriptorSet) != VK_SUCCESS)
	{
		return false;
	}

	std::vector<VkWriteDescriptorSet> descriptorWrites(config.bindings.size());
	std::vector<std::variant<VkDescriptorBufferInfo, VkDescriptorImageInfo>> descriptorInfos(config.bindings.size());

	size_t i{ 0 };
	for (const auto& binding : config.bindings)
	{
		auto& descriptorWrite		= descriptorWrites[i];
		descriptorWrite.sType		= VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrite.dstSet		= mDescriptorSet;
		descriptorWrite.dstBinding	= binding.binding;

		Common::visit(binding.descriptor, Common::Visitor{
			[&](Coral::Buffer* buffer)
			{ 
				auto bufferImpl		= static_cast<Coral::Vulkan::Buffer*>(buffer);

				VkDescriptorBufferInfo bufferInfo;
				bufferInfo.buffer	= bufferImpl->getVkBuffer();
				bufferInfo.offset	= 0;
				bufferInfo.range	= buffer->size();
				descriptorInfos[i]	= bufferInfo;

				descriptorWrite.pBufferInfo		= &std::get<VkDescriptorBufferInfo>(descriptorInfos[i]);
				descriptorWrite.descriptorType	= VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
				descriptorWrite.descriptorCount = 1;
			},
			[&](Coral::Sampler* sampler)
			{
				auto samplerImpl	= static_cast<Coral::Vulkan::Sampler*>(sampler);
				auto imageImpl		= static_cast<Coral::Vulkan::Image*>(sampler->image());

				VkDescriptorImageInfo imageInfo;
				imageInfo.sampler		= samplerImpl->getVkSampler();
				imageInfo.imageView		= imageImpl->getVkImageView();
				imageInfo.imageLayout	= VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;//  image->getVkImageLayout();
				descriptorInfos[i]		= imageInfo;

				descriptorWrite.pImageInfo		= &std::get<VkDescriptorImageInfo>(descriptorInfos[i]);
				descriptorWrite.descriptorType	= VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
				descriptorWrite.descriptorCount = 1;
			}
		});

		i++;
	}

	vkUpdateDescriptorSets(mContext->getVkDevice(), static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);

	return true;
}
