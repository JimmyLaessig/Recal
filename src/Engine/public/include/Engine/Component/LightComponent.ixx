module;

#include <memory>

export module Engine.Component.LightComponent;

export import Engine.Component;

import Engine.InheritsFrom;
import Engine.Light;

export namespace Reef
{

struct LinearColor
{
	float r{ 0.f };
	float g{ 0.f };
	float b{ 0.f };
	float a{ 1.f };
};

constexpr const LinearColor Black		= { 0.f, 0.f, 0.f, 1.f };
constexpr const LinearColor White		= { 1.f, 1.f, 1.f, 1.f };
constexpr const LinearColor Grey		= { 0.8f, 0.8f, 0.8f, 1.f };
constexpr const LinearColor DarkGrey	= { 0.5f, 0.5f, 0.5f, 1.f };
constexpr const LinearColor Red			= { 1.f, 0.0f, 0.0f, 1.f };
constexpr const LinearColor Green		= { 0.f, 1.0f, 0.0f, 1.f };
constexpr const LinearColor Blue		= { 1.f, 0.0f, 1.0f, 1.f };


class LightComponent : public InheritsFrom<LightComponent, Component>
{
public:

	/// The color of the light
	LinearColor color() const;

	/// Set the color of the light
	void setColor(const LinearColor& color);

	/// The brightness of the light (measured in Lux)
	float brightness() const;

	/// Set the brightness of the light in Lux
	void setBrightness(float brightness);

	virtual bool castsShadows() const;

	virtual void setCastsShadows(bool castsShadows);

	virtual std::shared_ptr<Light> light() = 0;

private:

	LinearColor mColor{ Engine::White };

	float mBrightness{ 1.0f };

	bool mCastsShadows{ false };
};

} // namespace Reef