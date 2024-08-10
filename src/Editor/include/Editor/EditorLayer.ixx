export module Editor.EditorLayer;

import Engine.Application.Layer;

namespace Editor
{

export class EditorLayer : public Engine::Layer
{

public:

	bool onInit() override;

	void deinit() override;

	void start() override;

	void beginFrame() override;

	void update() override;

	void endFrame() override;

	void render() override;

	void stop() override;

private:


};

} // namespace Editor