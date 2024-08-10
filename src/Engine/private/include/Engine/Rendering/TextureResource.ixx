module;

#include <memory>

export module Engine.Rendering.TextureResource;

import RenderLib.RAII;

import Engine.Texture;

namespace Reef
{

namespace Rendering
{

export class ResourceManager;

export class TextureResource
{
public:

	bool set(const Engine::Texture& texture, ResourceManager& manager);

	RenderLib::Sampler* sampler();

	RenderLib::Texture2D* texture();

private:

	RenderLib::RAII::Texture2DPtr mTexture;

	RenderLib::RAII::SamplerPtr mSampler;
};

} // Rendering

} // Engine
