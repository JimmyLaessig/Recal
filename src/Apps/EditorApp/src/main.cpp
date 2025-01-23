#include <Engine/Application/EntryPoint.hpp>
#include <Engine/Core/Scene.hpp>
#include <Engine/Core/SceneObject.hpp>

#include <Engine/Core/GLTFLoader.hpp>

#include <Engine/Component/CameraComponent.hpp>

#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <tiny_gltf.h>

#include <string>


class Loader : public Reef::SubclassOf<Loader, Reef::Component>
{
public:

	void onComponentCreated() override
	{
		auto sponza = Reef::GLTFLoader::load(owner().scene(), "C:/Users/bernh/Development/glTF-Sample-Assets/Models/Sponza/glTF/Sponza.gltf");
		//auto sponza = Reef::GLTFLoader::load(owner().scene(), "C:/Users/bernh/Development/glTF-Sample-Assets/Models/Box/glTF/Box.gltf");
		auto camera = createSceneObject("Camera");
		camera->addComponent<Reef::CameraComponent>(true);
	}
};


bool
Reef::initialize(Reef::Application& app, const Reef::CommandLineArguments& commandLineArguments)
{
	auto scene = app.getActiveScene();

	scene->createSceneObject("Loader")->addComponent<Loader>();

	
	return true;
}
