module;

#include <Coral/System.h>

#include <cstdint>

export module Coral.Texture;

export import Coral.Context;
export import Coral.Types;

export namespace Coral
{

class CORAL_API Texture
{
public:

	enum class Type
	{
		_2D,
		_3D,
	};

	virtual Type type() const = 0;

	virtual PixelFormat pixelFormat() const = 0;

	virtual uint32_t width() const = 0;

	virtual uint32_t height() const = 0;

	virtual bool hasMips() const = 0;

};

} // namespace Coral