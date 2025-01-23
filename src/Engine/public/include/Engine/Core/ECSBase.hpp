#ifndef ENGINE_CORE_ECSBASE_HPP
#define ENGINE_CORE_ECSBASE_HPP

#include <Engine/System.hpp>

#include <Engine/Core/SubclassOf.hpp>

#include <memory>
#include <vector>
#include <string>
#include <string_view>


namespace Reef
{

class Application;

class ENGINE_API ECSBase : public SubclassOf<ECSBase, Object>
{
public:

	void initialize(Application& application);

	const Application& application() const;

	Application& application();

private:

	Application* mApplication{ nullptr };
};

} // namespace Reef

#endif // !ENGINE_CORE_ECSBASE_HPP