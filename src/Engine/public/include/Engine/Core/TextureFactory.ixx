module;

#include <Engine/System.hpp>

#include <memory>
#include <string_view>

export module Engine.TextureFactory;

import Engine.Texture;


export namespace Reef
{

class ENGINE_API TextureFactory
{
public:

	static std::shared_ptr<Texture> createTexture(std::string_view path, bool flipVertically = false);
};

} // namespace Reef
