module;

#include <VulkanHeader.h>

#include <span>
#include <string_view>
#include <vector>

export module Coral.Vulkan.ShaderModule;

export import Coral.ShaderModule;

namespace Coral
{
namespace Vulkan
{
export class Context;

export class ShaderModule : public Coral::ShaderModule
{
public:

	virtual ~ShaderModule();

	bool init(Context* context, const Coral::ShaderModuleConfig& config);

	ShaderStage shaderStage() const override;

	std::string_view name() const override;

	std::string_view entryPoint() const override;

	std::span<const AttributeDescription> inputDescriptions() const override;

	std::span<const AttributeDescription> outputDescriptions() const override;

	std::span<const BindingDescription> bindingDescriptions() const override;

	VkShaderModule getVkShaderModule();
	
private:

	bool reflect(std::span<const std::byte> spirvCode);

	Context* mContext{ nullptr };

	std::string mName;

	std::vector<AttributeDescription> mInputDescriptions;

	std::vector<AttributeDescription> mOutputDescriptions;

	std::vector<BindingDescription> mBindingDescriptions;

	std::string mEntryPoint;

	Coral::ShaderStage mShaderStage{ Coral::ShaderStage::VERTEX };

	VkShaderModule mShaderModule{ VK_NULL_HANDLE };
};

} // namespace Vulkan

} // namespace Coral