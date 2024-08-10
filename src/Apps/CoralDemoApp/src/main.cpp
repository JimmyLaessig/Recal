#include "Shaders/UniformColor.h"

#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#pragma warning( push )
#pragma warning (disable: 4005)
#include <GLFW/glfw3native.h>
#pragma warning( pop )
#undef DELETE

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <array>
#include <memory>
#include <span>
#include <array>
#include <chrono>
#include <vector>
#include <string>
#include <unordered_map>

import Coral.RAII;

import Coral.UniformBlock;

namespace
{

struct Image
{
	std::vector<std::byte> data;

	uint32_t width{ 0 };
	uint32_t height{ 0 };

	Coral::PixelFormat format{ Coral::PixelFormat::RGBA8_UI };
};

bool loadImage(const char* path, Image& image)
{
	int x, y, c;
	stbi__vertically_flip_on_load = true;
	auto data = stbi_load(path, &x, &y, &c, 0);

	if (data == nullptr)
	{
		return false;
	}

	image.width		= static_cast<uint32_t>(x);
	image.height	= static_cast<uint32_t>(y);
	image.data.resize(x * y * c);
	std::memcpy(image.data.data(), data, image.data.size());

	stbi_image_free(data);

	switch (1)
	{
	case 1:
		switch (c)
		{
			case 1:	image.format = Coral::PixelFormat::R8_UI;		break;
			case 2:	image.format = Coral::PixelFormat::RG8_UI;		break;
			case 3:	image.format = Coral::PixelFormat::RGB8_UI;		break;
			case 4:	image.format = Coral::PixelFormat::RGBA8_UI;	break;
			default:
				return false;
		}
		break;
	case 2:
		switch (c)
		{
			case 1:	image.format = Coral::PixelFormat::R16_UI;		break;
			case 2:	image.format = Coral::PixelFormat::RG16_UI;		break;
			case 3:	image.format = Coral::PixelFormat::RGB16_UI;	break;
			case 4:	image.format = Coral::PixelFormat::RGBA16_UI;	break;
			default:
				return false;
		}
		break;
	}

	return true;
}

template<typename T>
Coral::AttributeFormat toAttributeFormat();

template<> Coral::AttributeFormat toAttributeFormat<glm::vec3>() { return Coral::AttributeFormat::VEC3; }
template<> Coral::AttributeFormat toAttributeFormat<glm::vec2>() { return Coral::AttributeFormat::VEC2; }
template<> Coral::AttributeFormat toAttributeFormat<uint32_t>() { return Coral::AttributeFormat::UINT32; }
template<> Coral::AttributeFormat toAttributeFormat<uint16_t>() { return Coral::AttributeFormat::UINT16; }

template<typename T>
std::pair<Coral::RAII::BufferPtr, Coral::RAII::BufferViewPtr>
createBuffer(Coral::Context* context, const std::span<const T>& elements, Coral::BufferType type)
{
	Coral::BufferConfig bufferConfig{};
	bufferConfig.size		= elements.size() * sizeof(T);
	bufferConfig.type		= type;
	bufferConfig.cpuVisible = false;
	auto buffer				= Coral::RAII::createBuffer(context, bufferConfig);

	Coral::BufferViewConfig bufferViewConfig{};
	bufferViewConfig.buffer			= buffer.get();
	bufferViewConfig.numElements	= elements.size();
	bufferViewConfig.offset			= 0;
	bufferViewConfig.stride			= 0;
	bufferViewConfig.attribute		= toAttributeFormat<T>();
	auto bufferView					= Coral::RAII::createBufferView(context, bufferViewConfig);

	auto queue = context->getTransferQueue();

	Coral::CommandBufferConfig commandBufferConfig{};
	commandBufferConfig.queue = queue;

	auto commandBuffer = Coral::RAII::createCommandBuffer(context, commandBufferConfig);

	Coral::UpdateBufferDataInfo updateInfo{};
	updateInfo.buffer = buffer.get();
	updateInfo.offset = 0;
	updateInfo.data	  = { reinterpret_cast<const std::byte*>(elements.data()), elements.size_bytes() };

	commandBuffer->begin();
	commandBuffer->cmdUpdateBufferData(updateInfo);
	commandBuffer->end();

	auto commandBufferPtr = commandBuffer.get();

	Coral::CommandBufferSubmitInfo submitInfo{};
	submitInfo.commandBuffers = { &commandBufferPtr , 1 };

	auto fence = Coral::RAII::createFence(context);
	queue->submit(submitInfo, fence.get());
	fence->wait();

	return { std::move(buffer), std::move(bufferView) };
}


void
updateUniformBuffer(Coral::Buffer& buffer, Coral::UniformBlock& block)
{
	auto mapped = buffer.map();
	auto data = block.data();
	std::memcpy(mapped, data.data(), data.size());

	buffer.unmap();
}


Coral::RAII::BufferPtr
createUniformBuffer(Coral::Context* context, Coral::UniformBlock& block)
{
	Coral::BufferConfig config{};
	config.cpuVisible	= true;
	config.size			= block.size();
	config.type			= Coral::BufferType::UNIFORM_BUFFER;

	auto buffer = Coral::RAII::createBuffer(context, config);

	updateUniformBuffer(*buffer, block);

	return std::move(buffer);
}


std::pair<Coral::RAII::ImagePtr, Coral::RAII::SamplerPtr>
createTexture(Coral::Context* context, const std::string& path)
{
	Image img;
	if (!loadImage("C:/Users/bernh/Development/Reef/uvtest.png", img))
	{
		return {};
	}

	Coral::ImageConfig imageConfig{};
	imageConfig.width	= img.width;
	imageConfig.height	= img.height;
	imageConfig.format	= img.format;
	imageConfig.hasMips = true;
	auto image = Coral::RAII::createImage(context, imageConfig);

	auto queue = context->getTransferQueue();

	Coral::CommandBufferConfig commandBufferConfig{};
	commandBufferConfig.queue = queue;

	auto commandBuffer = Coral::RAII::createCommandBuffer(context, commandBufferConfig);

	Coral::UpdateImageDataInfo updateInfo{};
	updateInfo.image		= image.get();
	updateInfo.updateMips	= image->getMipLevels() > 1;
	updateInfo.data			= { reinterpret_cast<const std::byte*>(img.data.data()), img.data.size() };

	commandBuffer->begin();
	commandBuffer->cmdUpdateImageData(updateInfo);
	commandBuffer->end();

	auto commandBufferPtr = commandBuffer.get();

	Coral::CommandBufferSubmitInfo submitInfo{};
	submitInfo.commandBuffers = { &commandBufferPtr , 1 };

	auto fence = Coral::RAII::createFence(context);
	queue->submit(submitInfo, fence.get());
	fence->wait();

	Coral::SamplerConfig samplerConfig{};
	samplerConfig.image		= image.get();
	samplerConfig.wrapMode	= Coral::WrapMode::REPEAT;
	auto sampler = Coral::RAII::createSampler(context, samplerConfig);

	return std::pair<Coral::RAII::ImagePtr, Coral::RAII::SamplerPtr>{ std::move(image), std::move(sampler) };
}


constexpr std::array<glm::vec3, 24> cubePositions = 
{
	// -z
	glm::vec3{  .5f, -.5f, -.5f },
	glm::vec3{ -.5f, -.5f, -.5f },
	glm::vec3{  .5f,  .5f, -.5f },
	glm::vec3{ -.5f,  .5f, -.5f },

	// +z
	glm::vec3{ -.5f, -.5f,  .5f },
	glm::vec3{  .5f, -.5f,  .5f },
	glm::vec3{ -.5f,  .5f,  .5f },
	glm::vec3{  .5f,  .5f,  .5f },

	// -x
	glm::vec3{ -.5f, -.5f, -.5f },
	glm::vec3{ -.5f, -.5f,  .5f },
	glm::vec3{ -.5f,  .5f, -.5f },
	glm::vec3{ -.5f,  .5f,  .5f },

	// +x
	glm::vec3{ .5f, -.5f,  .5f },
	glm::vec3{ .5f, -.5f, -.5f },
	glm::vec3{ .5f,  .5f,  .5f },
	glm::vec3{ .5f,  .5f, -.5f },

	// -y
	glm::vec3{ -.5f, -.5f, -.5f },
	glm::vec3{  .5f, -.5f, -.5f },
	glm::vec3{ -.5f, -.5f,  .5f },
	glm::vec3{  .5f, -.5f,  .5f },

	// +y
	glm::vec3{ -.5f, .5f,  .5f },
	glm::vec3{  .5f, .5f,  .5f },
	glm::vec3{ -.5f, .5f, -.5f },
	glm::vec3{  .5f, .5f, -.5f },
};


constexpr std::array<glm::vec2, 24> cubeTexcoords = 
{
	// -z
	glm::vec2{ 0.f, 0.f },
	glm::vec2{ 1.f, 0.f },
	glm::vec2{ 0.f, 1.f },
	glm::vec2{ 1.f, 1.f },

	// +z
	glm::vec2{ 0.f, 0.f },
	glm::vec2{ 1.f, 0.f },
	glm::vec2{ 0.f, 1.f },
	glm::vec2{ 1.f, 1.f },

	// -x
	glm::vec2{ 0.f, 0.f },
	glm::vec2{ 1.f, 0.f },
	glm::vec2{ 0.f, 1.f },
	glm::vec2{ 1.f, 1.f },

	// +x
	glm::vec2{ 0.f, 0.f },
	glm::vec2{ 1.f, 0.f },
	glm::vec2{ 0.f, 1.f },
	glm::vec2{ 1.f, 1.f },

	// -y
	glm::vec2{ 0.f, 0.f },
	glm::vec2{ 1.f, 0.f },
	glm::vec2{ 0.f, 1.f },
	glm::vec2{ 1.f, 1.f },

	// +y
	glm::vec2{ 0.f, 0.f },
	glm::vec2{ 1.f, 0.f },
	glm::vec2{ 0.f, 1.f },
	glm::vec2{ 1.f, 1.f },
};


constexpr std::array<glm::vec3, 24> cubeNormals =
{
	// -z
	glm::vec3{ 0.f, 0.f, -1.f },
	glm::vec3{ 0.f, 0.f, -1.f },
	glm::vec3{ 0.f, 0.f, -1.f },
	glm::vec3{ 0.f, 0.f, -1.f },

	// +z
	glm::vec3{ 0.f, 0.f, 1.f },
	glm::vec3{ 0.f, 0.f, 1.f },
	glm::vec3{ 0.f, 0.f, 1.f },
	glm::vec3{ 0.f, 0.f, 1.f },

	// -x
	glm::vec3{ -1.f, 0.f, 0.f },
	glm::vec3{ -1.f, 0.f, 0.f },
	glm::vec3{ -1.f, 0.f, 0.f },
	glm::vec3{ -1.f, 0.f, 0.f },

	// +x
	glm::vec3{ 1.f, 0.f, 0.f },
	glm::vec3{ 1.f, 0.f, 0.f },
	glm::vec3{ 1.f, 0.f, 0.f },
	glm::vec3{ 1.f, 0.f, 0.f },

	// -y
	glm::vec3{ 0.f, -1.f, 0.f },
	glm::vec3{ 0.f, -1.f, 0.f },
	glm::vec3{ 0.f, -1.f, 0.f },
	glm::vec3{ 0.f, -1.f, 0.f },

	// +y
	glm::vec3{ 0.f, 1.f, 0.f },
	glm::vec3{ 0.f, 1.f, 0.f },
	glm::vec3{ 0.f, 1.f, 0.f },
	glm::vec3{ 0.f, 1.f, 0.f },
};


constexpr std::array<uint16_t, 36> cubeIndices =
{
	// -z
	0, 1, 2,
	2, 1, 3,

	// +z
	4, 5, 6,
	6, 5, 7,

	// -x
	8, 9, 10,
	10, 9, 11,

	// +x
	12, 13, 14,
	14, 13, 15,

	// -y
	16, 17, 18,
	18, 17, 19,

	// +y
	20, 21, 22,
	22, 21, 23,
};

} // namespace

int main()
{
	if (glfwInit() != GLFW_TRUE)
	{
		return EXIT_FAILURE;
	}

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

	const auto WIDTH = 1024;
	const auto HEIGHT = 768;

	auto window = glfwCreateWindow(WIDTH, HEIGHT, "Coral Demo Application", nullptr, nullptr);
	
	glfwShowWindow(window);

	Coral::SurfaceConfig surfaceConfig{};
	surfaceConfig.nativeWindowHandle					= glfwGetWin32Window(window);
	surfaceConfig.swapchainConfig.depthFormat			= Coral::PixelFormat::DEPTH24_STENCIL8;
	surfaceConfig.swapchainConfig.format				= Coral::PixelFormat::RGBA8_SRGB;
	surfaceConfig.swapchainConfig.swapchainImageCount	= 2;

	Coral::ContextConfig contextConfig{};
	contextConfig.graphicsAPI	= Coral::GraphicsAPI::VULKAN;

	auto context = Coral::RAII::createContext(contextConfig);

	if (!context)
	{
		return EXIT_FAILURE;
	}

	auto swapchain = Coral::RAII::createSurface(context.get(), surfaceConfig);
	auto fence = Coral::RAII::createFence(context.get());
	auto queue = context->getGraphicsQueue();

	Coral::ShaderModuleConfig vertexShaderConfig{};
	vertexShaderConfig.name		= "UniformColorVertexShader";
	vertexShaderConfig.stage	= Coral::ShaderStage::VERTEX;
	vertexShaderConfig.source	= { (std::byte*)Shaders::UniformColor::vertexShaderSourceSpirV.data(),
									Shaders::UniformColor::vertexShaderSourceSpirV.size() * sizeof(uint32_t) };

	auto vertexShader = Coral::RAII::createShaderModule(context.get(), vertexShaderConfig);

	Coral::ShaderModuleConfig fragmentShaderConfig{};
	fragmentShaderConfig.name	= "UniformColorFragmentShader";
	fragmentShaderConfig.stage	= Coral::ShaderStage::FRAGMENT;
	fragmentShaderConfig.source = { (std::byte*)Shaders::UniformColor::fragmentShaderSourceSpirV.data(),
									Shaders::UniformColor::fragmentShaderSourceSpirV.size() * sizeof(uint32_t) };

	auto fragmentShader = Coral::RAII::createShaderModule(context.get(), fragmentShaderConfig);

	glm::mat4 modelMatrix(1.f);

	auto viewMatrix = glm::lookAt(glm::vec3(1, 1, 1), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));

	auto fov			  = glm::radians(65.f);
	auto nearPlane		  = 0.01f;
	auto farPlane		  = 1000.f;
	auto projectionMatrix = glm::perspective(fov, static_cast<float>(WIDTH) / HEIGHT, nearPlane, farPlane);

	auto vertexShaderBindings = vertexShader->bindingDescriptions();
	auto fragmentShaderBindings = fragmentShader->bindingDescriptions();

	Coral::UniformBlock lightData({{ 
		{ Coral::ValueType::VEC3F, "lightData.color", 1 }, 
		{ Coral::ValueType::VEC3F, "lightData.direction",  1 }
	}});

	lightData.setVec3F("lightData.color", glm::vec3{ 1.f, 1.f, 1.f });
	lightData.setVec3F("lightData.direction", glm::normalize(glm::vec3{ 1.f, 1.f, 1.f }));

	Coral::UniformBlock instanceData({{
		{ Coral::ValueType::MAT44F, "modelViewProjectionMatrix", 1 },
		{ Coral::ValueType::MAT33F, "normalMatrix",  1 }
	}});

	instanceData.setMat44F("modelViewProjectionMatrix", projectionMatrix * viewMatrix * modelMatrix);
	instanceData.setMat33F("normalMatrix", glm::mat3(glm::transpose(glm::inverse(modelMatrix))));

	auto lightDataUniform = createUniformBuffer(context.get(), lightData);
	auto instanceDataUniform = createUniformBuffer(context.get(), instanceData);

	auto [texture, sampler] = createTexture(context.get(), "C:/Users/bernh/Development/Reef/uvtest.png");

	std::vector bindings
	{
		Coral::DescriptorBinding{ 0, instanceDataUniform.get() },
		Coral::DescriptorBinding{ 1, lightDataUniform.get() },
		Coral::DescriptorBinding{ 2, sampler.get() },
	};

	struct DescriptorSetConfigData
	{
		std::vector<Coral::DescriptorBinding> bindings;
	};

	std::unordered_map<uint32_t, DescriptorSetConfigData> descriptorSetConfigData;

	descriptorSetConfigData[0].bindings = bindings;

	for (const auto& binding: vertexShaderBindings)
	{
		auto& data = descriptorSetConfigData[binding.binding];
		//Common::visit(binding.type)
		//binding.type
	}

	std::unordered_map<uint32_t, Coral::RAII::DescriptorSetPtr> descriptorSets;

	for (auto& [index, data] : descriptorSetConfigData)
	{
		Coral::DescriptorSetConfig descriptorSetConfig{};
		descriptorSetConfig.bindings	= data.bindings;
		descriptorSets[index]			= Coral::RAII::createDescriptorSet(context.get(), descriptorSetConfig);
	}

	auto [positionBuffer, positionBufferView]	= ::createBuffer(context.get(), std::span{ cubePositions.data(), cubePositions.size() }, Coral::BufferType::VERTEX_BUFFER);
	auto [normalBuffer, normalBufferView]		= ::createBuffer(context.get(), std::span{ cubeNormals.data(), cubeNormals.size() }, Coral::BufferType::VERTEX_BUFFER);
	auto [uvBuffer, uvBufferView]				= ::createBuffer(context.get(), std::span{ cubeTexcoords.data(), cubeTexcoords.size() }, Coral::BufferType::VERTEX_BUFFER);
	auto [indexBuffer, indexBufferView]			= ::createBuffer(context.get(), std::span{ cubeIndices.data(), cubeIndices.size() }, Coral::BufferType::INDEX_BUFFER);

	std::array shaderModules = { vertexShader.get(), fragmentShader.get() };

	Coral::PipelineStateConfig pipelineStateConfig{};
	pipelineStateConfig.shaderModules			= shaderModules;
	pipelineStateConfig.polygonMode				= Coral::PolygonMode::SOLID;
	pipelineStateConfig.topology				= Coral::Topology::TRIANGLE_LIST;
	pipelineStateConfig.faceCullingMode			= Coral::FaceCullingMode::BackFaceCulling;
	pipelineStateConfig.framebufferSignature	= swapchain->getSwapchainFramebuffer(0)->getSignature();

	auto pipelineState = Coral::RAII::createPipelineState(context.get(), pipelineStateConfig);

	auto before = std::chrono::system_clock::now();

	auto rotation = 0.f;
	auto rotationPerSecond = 90.f;

	// Start the game loop
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		{
			glfwSetWindowShouldClose(window, GLFW_TRUE);
		}

		swapchain->acquireNextSwapchainImage(fence.get());

		fence->wait();
		fence->reset();

		int width, height;
		glfwGetWindowSize(window, &width, &height);

		auto now = std::chrono::system_clock::now();
		auto duration = now - before;
		before = now;

		float deltaT = std::chrono::duration_cast<std::chrono::nanoseconds>(duration).count() / 1e+9f;

		rotation += rotationPerSecond * deltaT;

		modelMatrix = glm::rotate(glm::mat4(1), glm::radians(rotation), glm::vec3(0, 1, 0));

		projectionMatrix = glm::perspective(fov, static_cast<float>(width) / height, nearPlane, farPlane);

		instanceData.setMat44F("modelViewProjectionMatrix", projectionMatrix * viewMatrix * modelMatrix);
		instanceData.setMat33F("normalMatrix", glm::mat3(glm::transpose(glm::inverse(modelMatrix))));

		updateUniformBuffer(*instanceDataUniform, instanceData);

		auto index = swapchain->getCurrentSwapchainImageIndex();

		auto framebuffer = swapchain->getSwapchainFramebuffer(index);

		Coral::CommandBufferConfig commandBufferConfig{};
		commandBufferConfig.queue = queue;

		auto commandBuffer = Coral::RAII::createCommandBuffer(context.get(), commandBufferConfig);
		commandBuffer->begin();

		Coral::BeginRenderPassInfo beginRenderPassInfo{};
		beginRenderPassInfo.framebuffer = framebuffer;
		Coral::ClearColor clearColor	= { { 1, 1, 1, 1 }, 0 };
		Coral::ClearDepth clearDepth	= { 1.f, 0 };
		beginRenderPassInfo.clearColor	= { &clearColor, 1 };
		beginRenderPassInfo.clearDepth	= clearDepth;

		commandBuffer->cmdBeginRenderPass(beginRenderPassInfo);

		commandBuffer->cmdSetViewport({ 0, 0, static_cast<uint32_t>(width),  static_cast<uint32_t>(height),  0.f,  1.f , Coral::ViewportMode::Y_UP});

		commandBuffer->cmdBindPipeline(pipelineState.get());

		for (auto& [index, set] : descriptorSets)
		{
			commandBuffer->cmdBindDescriptorSet(set.get(), index);
		}
		commandBuffer->cmdBindVertexBuffer(positionBufferView.get(), 0);
		commandBuffer->cmdBindVertexBuffer(normalBufferView.get(), 1);
		commandBuffer->cmdBindVertexBuffer(uvBufferView.get(), 2);
		commandBuffer->cmdBindIndexBuffer(indexBufferView.get());

		Coral::DrawIndexInfo drawInfo{};
		drawInfo.firstIndex = 0;
		drawInfo.indexCount = indexBufferView->numElements();
		commandBuffer->cmdDrawIndexed(drawInfo);

		commandBuffer->cmdEndRenderPass();

		commandBuffer->end();

		Coral::CommandBufferSubmitInfo submitInfo{};
		auto cb = commandBuffer.get();
		submitInfo.commandBuffers = { &cb, 1 };

		queue->submit(submitInfo, fence.get());

		fence->wait();
		fence->reset();

		Coral::PresentInfo presentInfo{};
		presentInfo.surface = swapchain.get();
		queue->submit(presentInfo);

		queue->waitIdle();
	}

	return EXIT_SUCCESS;
}
