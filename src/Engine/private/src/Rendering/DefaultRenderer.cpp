#include <Engine/Rendering/DefaultRenderer.hpp>

#include <Engine/Application/Application.hpp>

#include <Engine/Rendering/Shaders/DefaultUniforms.h>

#include <Engine/Rendering/Shaders/DefaultPBR.hpp>

#include <Engine/Rendering/ShaderImpl.hpp>

#include <glm/gtc/matrix_transform.hpp>


#include <array>
#include <memory>
#include <ranges>
#include <vector>

using namespace Reef::Rendering;


bool
DefaultRenderer::initializeBuiltInShaders()
{
	auto shader = Reef::Rendering::ShaderImpl::create(BuiltInShaders::DefaultPBR,
													  std::as_bytes(std::span{ Shaders::DefaultPBR::vertexShaderSourceSpirV }),
													  std::as_bytes(std::span{ Shaders::DefaultPBR::fragmentShaderSourceSpirV }),
													  *mContext);
	if (!shader)
	{
		return false;
	}

	mShaders[BuiltInShaders::DefaultPBR] = std::move(shader);

	return true;
}


bool
DefaultRenderer::initialize(Reef::Application& application)
{
	mApplication = &application;

	Coral::ContextConfig contextConfig{};
	contextConfig.graphicsAPI = Coral::GraphicsAPI::VULKAN;

	mContext = Coral::RAII::createContext(contextConfig);

	if (!mContext)
	{
		return false;
	}

	auto window = application.window();

	if (!window)
	{
		return false;
	}

	Coral::SurfaceConfig surfaceConfig{};
	surfaceConfig.nativeWindowHandle					= window->nativeWindowHandle();
	surfaceConfig.swapchainConfig.depthFormat			= Coral::PixelFormat::DEPTH24_STENCIL8;
	surfaceConfig.swapchainConfig.format				= Coral::PixelFormat::RGBA8_SRGB;
	surfaceConfig.swapchainConfig.swapchainImageCount	= 2;

	mSurface = Coral::RAII::createSurface(mContext.get(), surfaceConfig);

	if (!mSurface)
	{
		return false;
	}

	mResourceManager.initialize(*mContext);

	if (!initializeBuiltInShaders())
	{
		return false;
	}

	return true;
}


std::shared_ptr<Shader>
DefaultRenderer::getShader(std::string_view name)
{
	auto iter = mShaders.find(std::string(name));

	if (iter == mShaders.end())
	{
		return nullptr;
	}

	return iter->second;
}


DefaultRenderer::SceneViewResolved
DefaultRenderer::updateResources(const SceneView& sceneView)
{
	
	auto viewMatrix = glm::lookAt(glm::vec3{ 100, 100, 100 }, glm::vec3{ 0, 100, 0 }, glm::vec3{ 0, 1, 0 });
	auto projectionMatrix = glm::perspective(glm::radians(60.f), sceneView.camera.size.x / static_cast<float>(sceneView.camera.size.y), 0.01f, 50000.f);

	mPerFrameUniforms.setMat44F("viewMatrix", viewMatrix);
	mPerFrameUniforms.setMat44F("projectionMatrix", projectionMatrix);
	mPerFrameUniforms.setMat44F("viewProjectionMatrix", projectionMatrix * viewMatrix);

	if (!mPerFrameUniformBuffer)
	{
		Coral::BufferConfig config{};
		config.cpuVisible = true;
		config.size = mPerFrameUniforms.size();
		config.type = Coral::BufferType::UNIFORM_BUFFER;

		mPerFrameUniformBuffer = Coral::RAII::createBuffer(mContext.get(), config);
	}

	if (mPerFrameUniformBuffer)
	{
		if (auto data = mPerFrameUniformBuffer->map())
		{
			auto span = mPerFrameUniforms.data();
			std::memcpy(data, span.data(), span.size_bytes());
			mPerFrameUniformBuffer->unmap();
		}
	}

	if (!mDescriptorSet)
	{
		Coral::DescriptorSetConfig config{};
		config.bindings = { Coral::DescriptorBinding{ 0u, mPerFrameUniformBuffer.get() } };

		mDescriptorSet = Coral::RAII::createDescriptorSet(mContext.get(), config);
	}

	SceneViewResolved res;

	auto resolveRenderObject = [&](const RenderObject& ro)
	{
		RenderObjectResolved res;

		res.materialResource = mResourceManager.getResource(*ro.material);
		res.primitive		 = mResourceManager.getResource(*ro.primitive);
		res.transformResouce = mResourceManager.getResource(ro.transform);

		return res;
	};

	res.renderObjects = sceneView.renderObjects
		| std::views::filter([](const RenderObject& ro) { return ro.primitive != nullptr && ro.material != nullptr; })
		| std::views::transform(resolveRenderObject)
		| std::ranges::to<std::vector>();

	return res; 
}


void
DefaultRenderer::render(Reef::Rendering::SceneView&& view)
{
	//view.camera.size.x,
	//view.camera.size.y;


	auto queue = mContext->getGraphicsQueue();

	auto fence = Coral::RAII::createFence(mContext.get());

	if (!mSurface->acquireNextSwapchainImage(fence.get()))
	{
		return;
	}

	fence->wait();

	auto index		 = mSurface->getCurrentSwapchainImageIndex();
	auto framebuffer = mSurface->getSwapchainFramebuffer(index);
	auto extent		 = mSurface->getSwapchainExtent();

	view.camera.size = { extent.width, extent.height };

	auto viewResolved = updateResources(view);
	
	Coral::CommandBufferConfig commandBufferConfig{};
	commandBufferConfig.queue = queue;

	auto commandBuffer = Coral::RAII::createCommandBuffer(mContext.get(), commandBufferConfig);
	commandBuffer->begin();

	Coral::BeginRenderPassInfo beginRenderPassInfo{};

	Coral::ClearColor clearColor = { { view.camera.clearColor.r, view.camera.clearColor.g, view.camera.clearColor.b, 1 }, 0 };
	Coral::ClearDepth clearDepth = { 1.f, 0 };
	beginRenderPassInfo.framebuffer = framebuffer;
	beginRenderPassInfo.clearColor  = { &clearColor, 1 };
	beginRenderPassInfo.clearDepth  = clearDepth;

	commandBuffer->cmdBeginRenderPass(beginRenderPassInfo);
	commandBuffer->cmdSetViewport({ 0, 0, extent.width, extent.height, 0.f, 1.f, Coral::ViewportMode::Y_UP });

	for (auto ro : viewResolved.renderObjects)
	{
		ro.materialResource->bind(*commandBuffer);

		commandBuffer->cmdBindDescriptorSet(mDescriptorSet.get(), PERFRAMEDATA_DESCRIPTORET);
		commandBuffer->cmdBindDescriptorSet(ro.transformResouce->getDescriptorSet(), PERINSTANCEDATA_DESCRIPTORSET);

		ro.primitive->bind(*commandBuffer);

		Coral::DrawIndexInfo drawInfo{};
		drawInfo.firstIndex = 0;
		drawInfo.indexCount = ro.primitive->indexCount();
		commandBuffer->cmdDrawIndexed(drawInfo);
	}

	commandBuffer->cmdEndRenderPass();
	commandBuffer->end();

	auto semaphore = Coral::RAII::createSemaphore(mContext.get());
	auto semaphorePtr = semaphore.get();

	Coral::CommandBufferSubmitInfo submitInfo{};
	Coral::CommandBuffer* cb	= commandBuffer.get();
	submitInfo.commandBuffers	= { &cb, 1 };
	submitInfo.signalSemaphores = { &semaphorePtr, 1 };

	queue->submit(submitInfo, nullptr);

	Coral::PresentInfo presentInfo{};
	presentInfo.surface			= mSurface.get();
	presentInfo.waitSemaphores  = { &semaphorePtr, 1 };
	
	queue->submit(presentInfo);

	queue->waitIdle();
}