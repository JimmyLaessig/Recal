module Engine.Component.LightComponent;

using namespace Reef;


Engine::LinearColor
LightComponent::color() const
{
	return mColor;
}


void
LightComponent::setColor(const LinearColor& color)
{
	mColor = color;
}


float
LightComponent::brightness() const
{
	return mBrightness;
}


void
LightComponent::setBrightness(float brightness)
{
	mBrightness = brightness;
}


bool
LightComponent::castsShadows() const
{
	return mCastsShadows;
}


void
LightComponent::setCastsShadows(bool castsShadows)
{
	mCastsShadows = castsShadows;
}