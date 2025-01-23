#include <Engine/Rendering/PrimitiveResource.hpp>

#include <glm/glm.hpp>

#include <cassert>
#include <vector>
#include <memory>
#include <span>

using namespace Reef::Rendering;

namespace
{

template<typename T>
std::tuple<Coral::RAII::BufferPtr, Coral::RAII::BufferViewPtr>
createBuffer(Coral::Context& context, std::span<const T> v, Coral::BufferType bufferType)
{
	Coral::AttributeFormat format;
	if constexpr (std::is_same_v<T, int16_t>)
	{
		format = Coral::AttributeFormat::INT16;
	}

	if constexpr (std::is_same_v<T, uint16_t>)
	{
		format = Coral::AttributeFormat::UINT16;
	}

	if constexpr (std::is_same_v<T, int32_t>)
	{
		format = Coral::AttributeFormat::INT32;
	}

	if constexpr (std::is_same_v<T, uint32_t>)
	{
		format = Coral::AttributeFormat::UINT32;
	}

	if constexpr (std::is_same_v<T, float>)
	{
		format = Coral::AttributeFormat::FLOAT;
	}

	if constexpr (std::is_same_v<T, glm::vec2>)
	{
		format = Coral::AttributeFormat::VEC2;
	}

	if constexpr (std::is_same_v<T, glm::vec3>)
	{
		format = Coral::AttributeFormat::VEC3;
	}

	if constexpr (std::is_same_v<T, glm::vec4>)
	{
		format = Coral::AttributeFormat::VEC4;
	}


	Coral::BufferConfig bufferConfig{};
	bufferConfig.size = v.size_bytes();
	bufferConfig.type = bufferType;

	auto buffer = Coral::RAII::createBuffer(&context, bufferConfig);
	if (!buffer)
	{
		return {};
	}

	Coral::BufferViewConfig config{};
	config.buffer	   = buffer.get();
	config.numElements = v.size();
	config.offset	   = 0;
	config.stride	   = 0;
	config.attribute   = format;

	auto view = Coral::RAII::createBufferView(&context, config);

	if (!view)
	{
		return {};
	}

	return { std::move(buffer), std::move(view) };
}

} // namespace


PrimitiveResource::PrimitiveResource(ResourceManager& resourceManager)
	: RenderResource(resourceManager)
{}


Coral::BufferView*
PrimitiveResource::indices()
{
	return mIndices.get();
}


Coral::BufferView*
PrimitiveResource::positions()
{
	return mPositions.get();
}


Coral::BufferView*
PrimitiveResource::normals()
{
	return mNormals.get();
}


Coral::BufferView*
PrimitiveResource::tangents()
{
	return mTangents.get();
}


Coral::BufferView*
PrimitiveResource::texcoords()
{
	return mTexcoords.get();
}


uint32_t
PrimitiveResource::indexCount()
{
	return mIndexCount;
}


void
PrimitiveResource::bind(Coral::CommandBuffer& commandBuffer)
{
	commandBuffer.cmdBindIndexBuffer(indices());

	uint32_t binding{ 0 };
	commandBuffer.cmdBindVertexBuffer(positions(), binding++);
	commandBuffer.cmdBindVertexBuffer(normals(), binding++);
	//commandBuffer->cmdBindVertexBuffer(ro.primitive->tangents(), binding++);
	commandBuffer.cmdBindVertexBuffer(texcoords(), binding++);
}


bool
PrimitiveResource::update(const Reef::Primitive& primitive, Coral::Context& context)
{
	if (!shouldUpdate(primitive.version()))
	{
		return true;
	}

	auto indices = primitive.indices();

	switch (primitive.indexFormat())
		{
		case IndexFormat::UNSIGNED_SHORT:
		{
			std::span data{ reinterpret_cast<const uint16_t*>(indices.data()), indices.size() / 2 };

			mIndexCount = data.size();
			std::tie(mIndexBuffer, mIndices) = createBuffer(context, data, Coral::BufferType::INDEX_BUFFER);
			break;
		}
		case IndexFormat::UNSIGNED_INT:
		{
			std::span data{ reinterpret_cast<const uint32_t*>(indices.data()), indices.size() / 4 };

			mIndexCount = data.size();
			std::tie(mIndexBuffer, mIndices) = createBuffer(context, data, Coral::BufferType::INDEX_BUFFER);
			break;
		}
	}

	std::tie(mPositionBuffer, mPositions) = createBuffer(context, primitive.positions(), Coral::BufferType::VERTEX_BUFFER);
	std::tie(mNormalBuffer, mNormals)	  = createBuffer(context, primitive.normals(), Coral::BufferType::VERTEX_BUFFER);
	std::tie(mTangentBuffer, mTangents)   = createBuffer(context, primitive.tangents(), Coral::BufferType::VERTEX_BUFFER);
	std::tie(mTexcoordBuffer, mTexcoords) = createBuffer(context, primitive.texcoords(), Coral::BufferType::VERTEX_BUFFER);

	auto queue = context.getTransferQueue();

	Coral::CommandBufferConfig commandBufferConfig{};
	commandBufferConfig.queue = queue;
	commandBufferConfig.name = "Update primitive resource";

	auto commandBuffer		= Coral::RAII::createCommandBuffer(&context, commandBufferConfig);
	auto comamndBufferPtr	= commandBuffer.get();
	
	commandBuffer->begin();

	Coral::UpdateBufferDataInfo updateBufferDataInfo{};
	updateBufferDataInfo.offset = 0;
	{
		updateBufferDataInfo.buffer = mIndexBuffer.get();
		updateBufferDataInfo.data = primitive.indices();
		commandBuffer->cmdUpdateBufferData(updateBufferDataInfo);
	}

	{
		updateBufferDataInfo.buffer = mPositionBuffer.get();
		updateBufferDataInfo.data = std::as_bytes(primitive.positions());
		commandBuffer->cmdUpdateBufferData(updateBufferDataInfo);
	}

	{
		updateBufferDataInfo.buffer = mNormalBuffer.get();
		updateBufferDataInfo.data = std::as_bytes(primitive.normals());
		commandBuffer->cmdUpdateBufferData(updateBufferDataInfo);
	}

	//{
	//	updateBufferDataInfo.buffer = mTangentBuffer.get();
	//	updateBufferDataInfo.data = std::as_bytes(primitive.tangents());
	//	commandBuffer->cmdUpdateBufferData(updateBufferDataInfo);
	//}

	{
		updateBufferDataInfo.buffer = mTexcoordBuffer.get();
		updateBufferDataInfo.data = std::as_bytes(primitive.texcoords());
		commandBuffer->cmdUpdateBufferData(updateBufferDataInfo);
	}

	commandBuffer->end();
	auto semaphore		= Coral::RAII::createSemaphore(&context);
	auto semaphorePtr	= semaphore.get();
	Coral::CommandBufferSubmitInfo submitInfo{};

	submitInfo.commandBuffers = { &comamndBufferPtr , 1 };
	submitInfo.signalSemaphores = { &semaphorePtr , 1 };
	queue->submit(submitInfo, nullptr);

	semaphore->wait();

	return true;
}
