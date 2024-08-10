module;

#include <variant>
#include <memory>

module Engine.Rendering.ResourceManager;
//
//
//import Engine.Rendering.TextureResource;
//import Engine.Rendering.PrimitiveResource;
//import Engine.Rendering.ShaderResource;
//
//import Engine.Rendering.UniformBufferFactory;
//import Engine.Rendering.SceneView;
//
//import RenderLib.RAII;
//
import Visitor;
//
//array>;
//vector>;

import Visitor;

using namespace Reef::Rendering;

bool
ResourceManager::initialize(RenderLib::Context& context)
{
	mContext = &context;

	return true;
}

PrimitiveResource* 
ResourceManager::getResource(const Engine::Primitive& primitive)
{
	auto iter = mResources.find(primitive.runtimeUniqueId());

	if (iter == mResources.end())
	{
		return nullptr;
	}

	return std::visit(Visitor
		{
			[&](std::unique_ptr<PrimitiveResource>& res) -> PrimitiveResource* { return res.get(); },
			[&](std::unique_ptr<TextureResource>& res) -> PrimitiveResource* { return nullptr; },
		}, iter->second);
}


TextureResource* 
ResourceManager::getResource(const Engine::Texture& texture)
{
	auto iter = mResources.find(texture.runtimeUniqueId());

	if (iter == mResources.end())
	{
		return nullptr;
	}
	
	return std::visit(Visitor
		{
			[&](std::unique_ptr<PrimitiveResource>& res) -> TextureResource* { return nullptr; },
			[&](std::unique_ptr<TextureResource>& res) -> TextureResource* { return res.get(); },
		}, iter->second);
}


TextureResource* 
ResourceManager::updateResource(const Engine::Texture& texture)
{
	auto resource = getResource(texture);

	bool updateResource = texture.isOutdated();

	if (!resource)
	{
		updateResource = true;
		
		auto newResource = std::make_unique<Engine::Rendering::TextureResource>();
		resource = newResource.get();
		mResources[texture.runtimeUniqueId()] = std::move(newResource);
	}

	if (!updateResource)
	{
		return resource;
	}

	if (!resource->set(texture, *this))
	{
		mResources.erase(texture.runtimeUniqueId());
		return nullptr;
	}

	return resource;
}


PrimitiveResource*  
ResourceManager::updateResource(const Engine::Primitive& primitive)
{
	auto resource = getResource(primitive);

	bool updateResource = primitive.isOutdated();

	if (!resource)
	{
		updateResource = true;

		auto newResource = std::make_unique<Engine::Rendering::PrimitiveResource>();
		resource = newResource.get();
		mResources[primitive.runtimeUniqueId()] = std::move(newResource);
	}

	if (!updateResource)
	{
		return resource;
	}

	if (!resource->set(primitive, *this))
	{
		mResources.erase(primitive.runtimeUniqueId());
		return nullptr;
	}

	return resource;
}


