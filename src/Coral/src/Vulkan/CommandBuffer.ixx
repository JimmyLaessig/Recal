module;

#include <VulkanHeader.h>

#include <memory>
#include <string>
#include <vector>

export module Coral.Vulkan.CommandBuffer;

export import Coral.CommandBuffer;

import Coral.Vulkan.Buffer;

namespace Coral
{

namespace Vulkan
{
export class Context;

class Image;
class PipelineState;
class DescriptorSet;
class PipelineLayout;

export class CommandBuffer : public Coral::CommandBuffer
{
public:

	virtual ~CommandBuffer();

	bool init(Context* context, const CommandBufferConfig& config);

	bool begin() override; 

	bool end() override;

	bool cmdBeginRenderPass(const BeginRenderPassInfo& info) override;

	bool cmdEndRenderPass() override;

	bool cmdClearImage(Coral::Image* image, const ClearColor& clearColor) override;

	bool cmdCopyBuffer(const CopyBufferInfo& info) override;

	bool cmdCopyImage(const CopyImageInfo& info) override;

	bool cmdBindVertexBuffer(Coral::BufferView* bufferView, uint32_t binding) override;

	bool cmdBindIndexBuffer(Coral::BufferView* bufferView) override;

	bool cmdBindPipeline(Coral::PipelineState* pipeline) override;

	bool cmdBindDescriptorSet(Coral::DescriptorSet* descriptorSet, uint32_t index) override;

	bool cmdDrawIndexed(const DrawIndexInfo& info) override;

	bool cmdSetViewport(const Coral::ViewportInfo& info) override;

	bool cmdUpdateBufferData(const Coral::UpdateBufferDataInfo& info) override;

	bool cmdUpdateImageData(const Coral::UpdateImageDataInfo& info) override;

	VkCommandBuffer getVkCommandBuffer();

	[[nodiscard]] std::vector<std::unique_ptr<Coral::Vulkan::Buffer>> getStagingBuffers();

private:

	void cmdAddImageBarrier(Coral::Vulkan::Image* image,
						    uint32_t baseMipLevel,
						    uint32_t levelCount,
						    VkImageLayout oldLayout,
						    VkImageLayout newLayout,
						    VkAccessFlagBits srcAccessMask,
						    VkAccessFlags dstAccessMask,
						    VkPipelineStageFlags srcStageFlags,
						    VkPipelineStageFlags dstStageFlags);

	std::vector<Coral::Vulkan::Image*> mPresentableImagesInUse;

	Context* mContext{ nullptr };

	VkCommandPool mCommandPool{ VK_NULL_HANDLE };

	VkCommandBuffer mCommandBuffer{ VK_NULL_HANDLE };

	std::string mName;

	PipelineState* mLastBoundPipelineState{ nullptr };

	std::vector<Coral::DescriptorSet*> mUnboundDescriptorSets;

	std::vector<std::unique_ptr<Coral::Vulkan::Buffer>> mStagingBuffers;

}; // class CommandBuffer

} // namespace Vulkan

} // namespace Coral