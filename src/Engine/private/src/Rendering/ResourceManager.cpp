#include <Engine/Rendering/ResourceManager.hpp>

#include <Common/Visitor.hpp>

#include <memory>
#include <variant>


using namespace Reef::Rendering;


void
ResourceManager::initialize(Coral::Context& context)
{
	mContext = &context;
}


PrimitiveResource*
ResourceManager::getResource(const Reef::Primitive& primitive)
{
	auto id = reinterpret_cast<size_t>(&primitive);
	auto iter = mResources.find(id);
	if (iter == mResources.end())
	{
		mResources[id] = std::make_unique<PrimitiveResource>(*this);
		iter = mResources.find(id);
	}

	return Common::visit(iter->second, Common::Visitor{
		[](const auto& ptr) -> PrimitiveResource* { return nullptr; },
		[&](const std::unique_ptr<PrimitiveResource>& ptr) -> PrimitiveResource*
		{
			return ptr->update(primitive, *mContext) ? ptr.get() : nullptr;
		}	
	});
}


MaterialResource*
ResourceManager::getResource(const Reef::Material& material)
{
	auto id = reinterpret_cast<size_t>(&material);
	auto iter = mResources.find(id);
	if (iter == mResources.end())
	{
		mResources[id] = std::make_unique<MaterialResource>(*this);
		iter = mResources.find(id);
	}

	return Common::visit(iter->second, Common::Visitor{ 
		[](const auto& ptr) -> MaterialResource* {  return nullptr; },
		[&](const std::unique_ptr<MaterialResource>& ptr) -> MaterialResource*
		{
			return ptr->update(material, *mContext) ? ptr.get() : nullptr;
		}
	});
}


TransformResource*
ResourceManager::getResource(const TransformData& transform)
{
	auto id = transform.objectId;
	auto iter = mResources.find(id);
	if (iter == mResources.end())
	{
		mResources[id] = std::make_unique<TransformResource>(*this);
		iter = mResources.find(id);
	}

	return Common::visit(iter->second, Common::Visitor{
		[](const auto& ptr) -> TransformResource* { return nullptr; },
		[&](const std::unique_ptr<TransformResource>& ptr) -> TransformResource*
		{
			return ptr->update(transform, *mContext) ? ptr.get() : nullptr;
		} });
}


TextureResource*
ResourceManager::getResource(const Reef::Texture& texture)
{
	auto id = reinterpret_cast<size_t>(&texture);
	auto iter = mResources.find(id);
	if (iter == mResources.end())
	{
		mResources[id] = std::make_unique<TextureResource>(*this);
		iter = mResources.find(id);
	}

	return Common::visit(iter->second, Common::Visitor{
		[](const auto& ptr) -> TextureResource* {  return nullptr; },
		[&](const std::unique_ptr<TextureResource>& ptr) -> TextureResource*
		{
			return ptr->update(texture, *mContext) ? ptr.get() : nullptr;
		}
		});
}