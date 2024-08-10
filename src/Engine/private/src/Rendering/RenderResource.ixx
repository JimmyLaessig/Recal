module;

#include <memory>
#include <unordered_map>

export module Engine.Rendering.RenderResource;

import Engine.Rendering.SceneView;

import Engine.Shader;

import RenderLib.RAII;

namespace Reef
{

namespace Rendering
{
export class ResourceManager;

export class RenderResource
{

public:

	bool set(const RenderObject& renderObject, const SceneView& view, ResourceManager& resourceManager);

private:

	RenderObject mRenderObject;

	PrimitiveResource* mPrimitiveResource{ nullptr };

	RenderLib::Shader* mShader{ nullptr };

	std::unordered_map<uint32_t, RenderLib::RAII::UniformBufferPtr> mUniformBuffers;
	
	std::unordered_map<uint32_t, RenderLib::Sampler> mUniformSamplers;

	RenderLib::RAII::DescriptorSetPtr mDescriptorSet;
};

} // Rendering

} // Engine
