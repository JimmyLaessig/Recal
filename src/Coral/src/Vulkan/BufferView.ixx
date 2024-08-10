module;

#include <cstdint>

export module Coral.Vulkan.BufferView;

export import Coral.BufferView;

import Coral.Types;

namespace Coral
{
namespace Vulkan
{

export class Buffer;
export class Context;

export class BufferView : public Coral::BufferView
{
public:

	bool init(Coral::Vulkan::Context* context, const Coral::BufferViewConfig& config);

	uint32_t offset() const override;

	uint32_t stride() const override;

	uint32_t numElements() const override;

	AttributeFormat attributeFormat() const override;

	Coral::Buffer* buffer() override;

	const Coral::Buffer* buffer() const override;

private:

	uint32_t mByteOffset{ 0 };

	uint32_t mStride{ 0 };

	uint32_t mNumElements{ 0 };

	AttributeFormat mAttributeFormat{ AttributeFormat::VEC3 };

	Coral::Vulkan::Buffer* mBuffer{ nullptr };
};

} // namespace GL

} // namespace Coral