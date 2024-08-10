module;

#include <Coral/System.h>

export module Coral.Sampler;

export namespace Coral
{

class Context;
class Image;

enum class Filter
{
	/// Return the texture element that is nearest to the specified texture coordinates
	NEAREST,
	/// Returns the weighted average of the four texture elements that are closest to the specified texture coordinates
	LINEAR
};


enum class WrapMode
{
	CLAMP_TO_EDGE,
	REPEAT,
	MIRROR,
	ONE,
	ZERO,
};


struct CORAL_API SamplerConfig
{
	Coral::Image* image{ nullptr };
	Filter minFilter{ Filter::LINEAR };
	Filter magFilter{ Filter::LINEAR };
	Filter mipmapFilter{ Filter::LINEAR };
	WrapMode wrapMode{ WrapMode::REPEAT };
};


class CORAL_API Sampler
{
public:

	virtual ~Sampler() = default;

	virtual Coral::Image* image() = 0;
};


CORAL_API Sampler* createSampler(Context* context, const SamplerConfig& config);

CORAL_API void destroy(Sampler* sampler);

} // namespace Coral