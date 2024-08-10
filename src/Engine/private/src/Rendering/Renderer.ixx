module;

#include <memory>
#include <vector>

export module Engine.Rendering.Renderer;

import Engine.Rendering.IRenderer;
import Engine.Rendering.ResourceManager;
import Engine.Rendering.ShaderManager;

import Multiton;

import RenderLib.RAII;

namespace Reef
{

namespace Rendering
{

export class Renderer : public IRenderer
{
public:

	bool initialize(RenderLib::Context& context);

	bool isInitialized();

	void submitCommandBuffer(RenderLib::CommandBuffer* commandBuffer);

	void execute();

	RenderLib::Context& context();

	ResourceManager& resourceManager();

	ShaderManager& shaderManager();

private:

	bool mIsInitialized{ false };

	RenderLib::Context* mContext{ nullptr };

	RenderLib::RAII::CommandQueuePtr mCommandQueue;

	ResourceManager mResourceManager;

	ShaderManager mShaderManager;
};

} // namespace Rendering

} // namespace Reef