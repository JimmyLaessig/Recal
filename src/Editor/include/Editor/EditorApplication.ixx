export module EditorApplication;

import Engine.Application;


namespace Editor
{

class EditorApplication : public Engine::Application
{

public:

	bool init(int argc, char** argv) override;
};

} // namespace Editor