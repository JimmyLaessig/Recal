#ifndef REEF_APPLICATION_LAYER_HPP
#define REEF_APPLICATION_LAYER_HPP

#include <Engine/System.hpp>
#include <Engine/Application/Application.hpp>
#include <Engine/Core/Object.hpp>
#include <Engine/Core/SubclassOf.hpp>

namespace Reef
{


class ENGINE_API Layer : public SubclassOf<Layer, Object>
{
public:

	virtual void start();

	virtual void update();

	Application& application();

	const Application& application() const;

private:

	Application* mApplication{ nullptr };
};

} // namespace Reef

#endif // !REEF_APPLICATION_LAYER_HPP