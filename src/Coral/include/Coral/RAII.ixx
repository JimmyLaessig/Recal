module;

#include <Coral/System.h>

#include <memory>

export module Coral.RAII;

export import Coral;

export namespace Coral
{

export namespace RAII
{

struct CORAL_API Deleter
{
	template<typename T>
	void operator()(T* t)
	{
		Coral::destroy(t);
	}
};

using BufferPtr					= std::unique_ptr<Buffer, Deleter>;
using BufferViewPtr				= std::unique_ptr<BufferView, Deleter>;
using ContextPtr				= std::unique_ptr<Context, Deleter>;
using CommandBufferPtr			= std::unique_ptr<CommandBuffer, Deleter>;
using SurfacePtr				= std::unique_ptr<Surface, Deleter>;
using SemaphorePtr				= std::unique_ptr<Semaphore, Deleter>;
using FencePtr					= std::unique_ptr<Fence, Deleter>;
using PipelineStatePtr			= std::unique_ptr<PipelineState, Deleter>;
using SamplerPtr				= std::unique_ptr<Sampler, Deleter>;
using ShaderModulePtr			= std::unique_ptr<ShaderModule, Deleter>;
using DescriptorSetPtr			= std::unique_ptr<DescriptorSet, Deleter>;
using ImagePtr					= std::unique_ptr<Image, Deleter>;

CORAL_API ImagePtr createImage(Context* context, const ImageConfig& config);

CORAL_API BufferPtr createBuffer(Context* context, const BufferConfig& config);

CORAL_API BufferViewPtr createBufferView(Context* context, const BufferViewConfig& config);

CORAL_API ShaderModulePtr createShaderModule(Context* context, const ShaderModuleConfig& config);

CORAL_API SemaphorePtr createSemaphore(Context* context);

CORAL_API CommandBufferPtr createCommandBuffer(Context* context, const CommandBufferConfig& config);

CORAL_API ContextPtr createContext(const ContextConfig& config);

CORAL_API SamplerPtr createSampler(Context* context, const SamplerConfig& config);

CORAL_API SurfacePtr createSurface(Context* context, const SurfaceConfig& config);

CORAL_API FencePtr createFence(Context* context);

CORAL_API PipelineStatePtr createPipelineState(Context* context, const PipelineStateConfig& config);

CORAL_API ShaderModulePtr createShaderModule(Context* context, const ShaderModuleConfig& config);

CORAL_API DescriptorSetPtr createDescriptorSet(Context* context, const DescriptorSetConfig& config);

} // namespace RAII

} // namespace Coral