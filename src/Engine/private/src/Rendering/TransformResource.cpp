#include <Engine/Rendering/TransformResource.hpp>

#include <glm/glm.hpp>

#include <cassert>
#include <vector>
#include <memory>
#include <span>

using namespace Reef::Rendering;


TransformResource::TransformResource(ResourceManager& resourceManager)
	: RenderResource(resourceManager)
{}


Coral::DescriptorSet*
TransformResource::getDescriptorSet()
{
	return mDescriptorSet.get();
}


bool
TransformResource::update(const TransformData& transform, Coral::Context& context)
{
	mBuilder.setMat44F("modelMatrix", transform.localToWorldMatrix);
	mBuilder.setMat44F("inverseModelMatrix", transform.worldToLocalMatrix);
	mBuilder.setMat33F("normalMatrix", glm::transpose(glm::inverse(glm::mat3x3(transform.localToWorldMatrix))));

	if (!mUniformBuffer)
	{
		Coral::BufferConfig config{};
		config.cpuVisible	= true;
		config.size			= mBuilder.size();
		config.type			= Coral::BufferType::UNIFORM_BUFFER;

		mUniformBuffer = Coral::RAII::createBuffer(&context, config);
	}

	if (!mUniformBuffer)
	{
		return false;
	}

	if (auto mapped = mUniformBuffer->map())
	{
		std::memcpy(mapped, mBuilder.get(), mBuilder.size());
		mUniformBuffer->unmap();
	}

	if (!mDescriptorSet)
	{
		Coral::DescriptorSetConfig config{};

		config.bindings = { Coral::DescriptorBinding{ 0, mUniformBuffer.get() } };

		mDescriptorSet = Coral::RAII::createDescriptorSet(&context, config);
	}

	if (!mDescriptorSet)
	{
		return false;
	}

	return true;
}


bool
TransformResource::bind(Coral::CommandBuffer& commandBuffer)
{
	return commandBuffer.cmdBindDescriptorSet(mDescriptorSet.get(), 0);
}
