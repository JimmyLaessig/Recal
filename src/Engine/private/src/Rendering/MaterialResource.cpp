//module Engine.Rendering.MaterialResource;
//
//import Engine.Rendering.ResourceManager;
//
//import Engine.Rendering.UniformBufferFactory;
//
//import Visitor;
//
//using namespace Reef::Rendering;
//
//
//bool
//MaterialResource::set(const Engine::Material& material, Engine::Rendering::ResourceManager& resourceManager)
//{
//	auto context = resourceManager.context();
//
//		//	// TODO: Supply these via a separate DescriptorSet
//	//	ro.material->setParameter("viewMatrix",						view.viewMatrix);
//	//	ro.material->setParameter("projectionMatrix",				view.camera.projectionMatrix);
//	//	ro.material->setParameter("viewProjectionMatrix",			view.viewProjectionMatrix);
//
//	//	ro.material->setParameter("normalMatrix",					ro.normalMatrix);
//	//	ro.material->setParameter("modelMatrix",					ro.localToWorldMatrix);
//	//	ro.material->setParameter("modelViewProjectionMatrix",		view.viewProjectionMatrix * ro.localToWorldMatrix);
//	//	ro.material->setParameter("inverseViewProjectionMatrix",	Math::inverse(view.viewProjectionMatrix));
//
//	//	//RenderLib::DescriptorSetConfig modelUniformsConfig{};
//	//	//modelUniformsConfig.uniformBuffers;
//	//	//modelUniformsConfig.uniformSamplers;
//	//	//auto modelUniforms = RenderLib::createDescriptorSet(mContext, modelUniformsConfig);
//	//	//commandBuffer->cmdBindDescriptorSet(modelUniforms);
//
//	//	//uniformBuffers.clear();
//	//	//uniformSamplers.clear();
//
//	
//	//	std::vector<RenderLib::UniformSamplerDescriptor> uniformSamplers;*/
//
//	for (auto& [location, value] : material.parameters())
//	{
//		std::visit(Visitor{
//			[&](float v)			 { mUniformBuffers[location] = UniformBufferFactory::createUniformBuffer(*context, v); },
//			[&](const Math::Vec2& v) { mUniformBuffers[location] = UniformBufferFactory::createUniformBuffer(*context, v); },
//			[&](const Math::Vec3& v) { mUniformBuffers[location] = UniformBufferFactory::createUniformBuffer(*context, v); },
//			[&](const Math::Vec4& v) { mUniformBuffers[location] = UniformBufferFactory::createUniformBuffer(*context, v); },
//
//			[&](int v)				 { mUniformBuffers[location] = UniformBufferFactory::createUniformBuffer(*context, v); },
//			[&](const glm::ivec2& v){ mUniformBuffers[location] = UniformBufferFactory::createUniformBuffer(*context, v); },
//			[&](const glm::ivec3& v){ mUniformBuffers[location] = UniformBufferFactory::createUniformBuffer(*context, v); },
//			[&](const glm::ivec4& v){ mUniformBuffers[location] = UniformBufferFactory::createUniformBuffer(*context, v); },
//
//			[&](const glm::mat2& v) { mUniformBuffers[location] = UniformBufferFactory::createUniformBuffer(*context, v); },
//			[&](const glm::mat3& v) { mUniformBuffers[location] = UniformBufferFactory::createUniformBuffer(*context, v); },
//			[&](const glm::mat4& v) { mUniformBuffers[location] = UniformBufferFactory::createUniformBuffer(*context, v); },
//
//			[&](std::shared_ptr<Texture> texture) { },
//			}, value);
//	}
//
//	std::vector<RenderLib::UniformBufferDescriptor> uniformBuffers;
//	std::vector<RenderLib::UniformSamplerDescriptor> uniformSamplers;
//
//
//	for (auto& [location, uniform] : mUniformBuffers)
//	{
//		uniformBuffers.push_back(RenderLib::UniformBufferDescriptor{ location, uniform.get() });
//	}
//
//	//for (auto& [location, uniform] : material.parameters())
//	//{
//	//	uniformBuffers.push_back(RenderLib::UniformBufferDescriptor{ location, uniform.get() });
//	//}
//
//	RenderLib::DescriptorSetConfig config{};
//	config.uniformBuffers	= uniformBuffers;
//	config.uniformSamplers	= uniformSamplers;
//	mDescriptorSet	= RenderLib::RAII::createDescriptorSet(context, config);
//
//	return true;
//}