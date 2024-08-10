module;

#include <VulkanHeader.h>

export module Coral.Vulkan.Buffer;

export import Coral.Buffer;

namespace Coral
{

namespace Vulkan
{
export class Context;

export class Buffer : public Coral::Buffer
{
public:

	virtual ~Buffer();

	bool init(Context* context, const BufferConfig& config);

	VkBuffer getVkBuffer();

	size_t size() const override;

	BufferType type() const override;

	std::byte* map() override;

	bool unmap() override;

private:

	Context* mContext{ nullptr };

	VkBuffer mBuffer{ VK_NULL_HANDLE };

	VmaAllocation mAllocation{ VK_NULL_HANDLE };

	BufferType mType{ BufferType::STORAGE_BUFFER };

	size_t mSize{ 0 };

	bool mCpuVisible{ false };

	std::byte* mMapped{ nullptr };

}; // class Buffer

} // namespace Vulkan

} // namespace Coral