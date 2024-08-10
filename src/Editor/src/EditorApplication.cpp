module;

#include <memory>

module EditorApplication;

import Engine.Application.WindowLayer;
import Engine.Application.SceneLayer;
import Engine.Application.ImGUILayer;
import Editor.EditorLayer;
using namespace Editor;


bool
EditorApplication::init(int argc, char** argv)
{
	addLayer(std::make_unique<Engine::WindowLayer>());
	//addLayer(std::make_unique<Engine::SceneLayer>());
	addLayer(std::make_unique<Engine::ImGUILayer>());
	addLayer(std::make_unique<Editor::EditorLayer>());

	return true;
}
