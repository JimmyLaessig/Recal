module;

#include <glm/glm.hpp>

#include <memory>
#include <unordered_map>

export module Engine.Rendering.DefaultRenderer;

import Engine.Renderer;

import Coral.RAII;

export namespace Reef
{
class IApplication;

class DefaultRenderer : public IRenderer
{
public:

	bool initialize(IApplication& application);

	void render() override;

private:

	bool initializeDefaultShaders();

	IApplication* mApplication{ nullptr };

	Coral::RAII::ContextPtr mContext;

	Coral::RAII::SurfacePtr mSurface;
};

} // namespace Reef