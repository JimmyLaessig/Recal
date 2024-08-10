module;

#include <VulkanHeader.h>

export module Coral.Vulkan.Image;

export import Coral.Image;

namespace Coral
{

namespace Vulkan
{
export class Context;

export class Image : public Coral::Image
{
public:

	virtual ~Image();

	bool init(Context* context, VkImage image, Coral::PixelFormat format, uint32_t width, uint32_t height, uint32_t mipLevelCount, VkImageLayout layout);

	bool init(Context* context, const Coral::ImageConfig& config);

	VkImage getVkImage();

	VkImageView getVkImageView();

	VkImageLayout getVkImageLayout() const;
	
	uint32_t width() const override;

	uint32_t height() const override;

	Coral::PixelFormat format() const override;

	uint32_t getMipLevels() const override;

	bool presentable() const override;

private:

	Context* mContext{ nullptr };

	VkImage mImage{ VK_NULL_HANDLE };

	VkImageView mImageView{ VK_NULL_HANDLE };

	VmaAllocation mAllocation{ VK_NULL_HANDLE };

	uint32_t mWidth{ 0 };

	uint32_t mHeight{ 0 };

	Coral::PixelFormat mFormat{ };

	uint32_t mMipLevelCount{ 1 };

	VkImageLayout mLayout{ VK_IMAGE_LAYOUT_UNDEFINED };

	bool mIsOwner{ false };
};

} // namespace Vulkan

} // namespace Coral
