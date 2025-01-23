#ifndef ENGINE_RENDERING_TEXTURERESOURCE_HPP
#define ENGINE_RENDERING_TEXTURERESOURCE_HPP

#include <memory>

#include <Engine/Rendering/RenderResource.hpp>

//import Engine.Rendering.RenderResource;

#include <Engine/Core/Texture.hpp>

#include <Coral/RAII.hpp>

namespace Reef::Rendering
{

class ResourceManager;

class TextureResource : public RenderResource
{
public:

	TextureResource(ResourceManager& resourceManager);

	Coral::Sampler* sampler();

	Coral::Image* texture();

	bool update(const Reef::Texture& texture, Coral::Context& context);

private:

	Coral::RAII::ImagePtr mTexture{ nullptr };
	Coral::RAII::SamplerPtr mSampler{ nullptr };
};

} // Reef::Rendering 

#endif //! ENGINE_RENDERING_TEXTURERESOURCE_HPP
