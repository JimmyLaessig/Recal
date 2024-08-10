module;

#include <VulkanHeader.h>

export module Coral.Vulkan.Sampler;

export import Coral.Sampler;

namespace Coral
{
namespace Vulkan
{

export class Context;
export class Image;

export class Sampler : public Coral::Sampler
{
public:

	bool init(Coral::Vulkan::Context* context, const Coral::SamplerConfig& config);

	virtual ~Sampler();

	Coral::Image* image() override;
	
	VkSampler getVkSampler();

private:
	
	Context* mContext{ nullptr };

	Image* mImage{ nullptr };

	Coral::Filter mMinFilter{ Coral::Filter::NEAREST };

	Coral::Filter mMagFilter{ Coral::Filter::NEAREST };

	Coral::Filter mMipmapFilter{ Coral::Filter::NEAREST };

	Coral::WrapMode mWrapMode{ Coral::WrapMode::REPEAT };

	VkSampler mSampler{ VK_NULL_HANDLE };
};

} // namespace Vulkan

} // namespace Coral