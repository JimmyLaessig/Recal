export module Engine.ImageLoader;

//#include <Engine/System.h>

import Engine.Image;

namespace Reef
{

class ImageLoader
{
public:

	static bool loadImage(const char* path, Reef::Image& image);

};

} // namespace Reef