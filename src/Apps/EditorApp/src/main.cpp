
#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <tiny_gltf.h>


#include <string>

import Engine.Application;
import Engine.Application.CommandLineArguments;
//import Engine.SubclassOf;
//import Engine.Component;

import Engine.GameObjectFactory;

//class UpdateComponent : public Reef::SubclassOf<UpdateComponent, Reef::Component>
//{
//public:
//
//	void onUpdate()
//	{
//		//std::cout << "onFrameUpdate" << std::endl;
//	}
//};


int main(int argc, char** argv)
{
	Reef::CommandLineArguments arguments(static_cast<size_t>(argc), argv);

	auto app = Reef::createApplication(arguments);

	auto scene = app->getActiveScene();

	tinygltf::Model model;
	std::string err;
	std::string warn;
	tinygltf::TinyGLTF loader;
	//loader.SetImageLoader(nullptr, nullptr);
	bool success = loader.LoadASCIIFromFile(&model, &err, &warn, "C:/Users/bernh/Development/glTF-Sample-Assets/Models/Sponza/glTF/Sponza.gltf");
	
	Reef::GameObjectFactory::createGameObject(*scene, model);

	auto returnCode = app->run();

	return static_cast<int>(returnCode);
}