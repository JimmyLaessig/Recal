module;

#include <glm/glm.hpp>
#include <glm/gtx/matrix_decompose.hpp>

#include <tiny_gltf.h>

#include <execution>
#include <filesystem>
#include <utility>

module Engine.GameObjectFactory;

import Engine.Component.MeshComponent;

import Engine.Material;
import Engine.Mesh;
import Engine.MeshFactory;
import Engine.Texture;
import Engine.TextureFactory;

using namespace Reef;

namespace
{

struct TRS
{
	glm::vec3 translation{ 0.f, 0.f, 0.f };
	glm::quat rotation;
	glm::vec3 scale{ 1.f, 1.f, 1.f };
};


TRS
getTransform(const tinygltf::Node& node)
{
	glm::vec3 position;
	glm::vec3 scale;
	glm::quat rotation;
	glm::vec3 skew;
	glm::vec4 perspective;
	glm::decompose(glm::mat4(1.f), scale, rotation, position, skew, perspective);

	return { position , rotation, scale };
}


glm::vec4
toVec4(const std::vector<double>& v)
{
	glm::vec4 vec;

	for (size_t i = 0; i < std::min<size_t>(v.size(), 4u); ++i)
	{
		vec[i] = v[i];
	}

	return vec;
}

} // namespace


GameObject*
GameObjectFactory::createGameObject(Scene& scene, const tinygltf::Model& model)
{
	auto root = scene.createGameObject();
	root->setName("Root");
	std::vector<GameObject*> gameObjects;

	std::vector<std::shared_ptr<Reef::Mesh>> meshes;
	std::vector<std::shared_ptr<Reef::Texture>> textures;
	std::vector<std::shared_ptr<Reef::Material>> materials;

	textures.resize(model.textures.size());
	
	std::vector<size_t> indices; 
	indices.resize(model.textures.size());
	for (size_t i = 0; i < model.textures.size(); ++i)
	{
		indices.push_back(i);
	}

	//Convert the gltf texture to Engine texture
	//std::for_each(std::execution::par, indices.begin(), indices.end(), [&](const auto& index)
	//{
	//	auto& texture = model.textures[index];
	//	auto image = model.images[model.source];

	//	std::filesystem::path imagePath(image.uri);
	//	std::filesystem::path workingDir(model.path);
	//	workingDir.remove_filename();
	//	if (imagePath.is_relative())
	//	{
	//		imagePath = workingDir / imagePath;
	//	}

	//	auto tex = TextureFactory::createTexture(imagePath.string());
	//	tex->setName(image.name);
	//	textures[index] = tex;
	//});

	//Convert the gltf material to Engine material
	for (const tinygltf::Material& material : model.materials)
	{
		auto mat = std::make_shared<Material>();
		//mat->setShader(Engine::Rendering::Renderer::instance().shaderManager().find("DefaultPBR"));
		
		mat->setParameter("baseColorFactor",	toVec4(material.pbrMetallicRoughness.baseColorFactor));
		/*mat->setParameter("baseColorTexture",	material.baseColorTexture	? textures[material.baseColorTexture->index]	: Engine::Texture::white());
		mat->setParameter("normalTexture",		material.normalTexture		? textures[material.normalTexture->index]		: Engine::Texture::normal());
		mat->setParameter("occlusionTexture",	material.occlusionTexture	? textures[material.occlusionTexture->index]	: Engine::Texture::black());*/
		//mat->setParameter("occlusionChannel",	0);

		//mat->setParameter("roughnessTexture",	material.roughnessMetallicTexture ? textures[material.roughnessMetallicTexture->index] : Engine::Texture::white());
		//mat->setParameter("roughnessChannel",	1);
		//mat->setParameter("roughnessFactor",	material.roughnessFactor.value_or(1.0f));
		//mat->setParameter("metallicTexture",	material.roughnessMetallicTexture ? textures[material.roughnessMetallicTexture->index] : Engine::Texture::white());
		//mat->setParameter("metallicChannel",	2);
		//mat->setParameter("metallicFactor",		material.roughnessFactor.value_or(0.0f));
		
		materials.push_back(mat);
	}

	// Convert the gltf mesh to Reef mesh
	for (const auto& gltfMesh : model.meshes)
	{
		auto mesh = MeshFactory::createMesh(gltfMesh, model);

		uint32_t i = 0;
		for (auto primitive : gltfMesh.primitives)
		{
			auto material = materials[primitive.material];

			mesh->getPrimitive(i)->setMaterial(material);
			i++;
		}

		meshes.push_back(mesh);
	}

	// Create all meshes
	uint32_t nodeIndex = 0;
	for (const auto& node : model.nodes)
	{
		auto go = scene.createGameObject();
		gameObjects.push_back(go);

		go->setParent(root, false);
		go->setName("Node_" + std::to_string(nodeIndex++));
		
		auto [translation, rotation, scale] = getTransform(node);

		go->transform().setLocalPose(translation, rotation, scale);

		if (node.mesh >= 0)
		{
			auto meshComponent = go->addComponent<MeshComponent>();
			const auto& mesh = meshes[node.mesh];
			meshComponent->setMesh(mesh);
		}
	}
	
	// Rebuild the scene graph
	uint32_t i = 0;
	for (const auto& node : model.nodes)
	{
		auto parent = gameObjects[i];

		for (auto childIndex : node.children)
		{
			auto child = gameObjects[childIndex];

			child->setParent(parent, false);
		}

		i++;
	}

	return root;
}


//GameObject*
//GameObjectFactory::createSkybox(Scene& scene, std::shared_ptr<Engine::Texture> skyboxTexture)
//{
//	/*auto mesh = std::make_shared<Engine::Mesh>();
//	
//	auto& quad = mesh->addPrimitive();
//
//	std::array positions
//	{
//		glm::vec3(-1.f, -1.f, 1.f),
//		glm::vec3(1.f, -1.f, 1.f),
//		glm::vec3(-1.f, 1.f, 1.f),
//		glm::vec3(1.f, 1.f, 1.f),
//	};
//
//	std::array<uint16_t, 6> indices
//	{
//		0, 1, 2,
//		2, 1, 3
//	};
//
//	std::array normals =
//	{
//		glm::vec3(0.f, 0.f, 1.f),
//		glm::vec3(0.f, 0.f, 1.f),
//		glm::vec3(0.f, 0.f, 1.f),
//		glm::vec3(0.f, 0.f, 1.f),
//	};
//
//	std::array tangents =
//	{
//		glm::vec3(1.f, 0.f, 0.f),
//		glm::vec3(1.f, 0.f, 0.f),
//		glm::vec3(1.f, 0.f, 0.f),
//		glm::vec3(1.f, 0.f, 0.f),
//	};
//
//	quad.setPositions(positions);
//	quad.setIndices(indices);
//	quad.setNormals(normals);
//	quad.setTangents(tangents);
//
//	auto material = std::make_shared<Engine::Material>();
//	material->setShader(Engine::Rendering::Renderer::instance().shaderManager().find("UnlitSkybox"));
//	material->setParameter("skyboxTexture", skyboxTexture);
//	material->setCastsShadow(false);
//	quad.setMaterial(material);
//
//	auto go = scene.createGameObject("Skybox");
//	auto component = go->addComponent<Engine::MeshComponent>();
//	component->setMesh(mesh);
//
//	auto environmentLight = go->addComponent<Engine::SkyLightComponent>();
//	environmentLight->setEnvironmentTexture(skyboxTexture);
//	return go;*/
//	return nullptr;
//}


//GameObject*
//GameObjectFactory::createCoordinateCross(Scene& scene, float scale)
//{
	/*auto go = scene.createGameObject("Axes");
	auto x = scene.createGameObject("X");
	auto y = scene.createGameObject("Y");
	auto z = scene.createGameObject("Z");
	
	float s1 = scale;
	float s2 = scale * 0.1f;

	x->transform().setLocalScale({ s1, s2, s2 });
	y->transform().setLocalScale({ s2, s1, s2 });
	z->transform().setLocalScale({ s2, s2, s1 });

	x->transform().setLocalPosition({  s1 * 0.5f, 0.f, 0.f });
	y->transform().setLocalPosition({ 0.f,  s1 * 0.5f, 0.f });
	z->transform().setLocalPosition({ 0.f, 0.f,  s1 * 0.5f });

	auto compX = x->addComponent<Engine::MeshComponent>(); 
	auto compY = y->addComponent<Engine::MeshComponent>(); 
	auto compZ = z->addComponent<Engine::MeshComponent>(); 

	compX->setMesh(Engine::Mesh::Cube());
	compY->setMesh(Engine::Mesh::Cube());
	compZ->setMesh(Engine::Mesh::Cube());

	auto cube = Engine::Mesh::Cube();
	auto material = cube->material(0);

	auto materialX = std::make_shared<Material>(*material);
	auto materialY = std::make_shared<Material>(*material);
	auto materialZ = std::make_shared<Material>(*material);

	materialX->setParameter("baseColorFactor", glm::vec4(1.f, 0.f, 0.f, 1.f));
	materialY->setParameter("baseColorFactor", glm::vec4(0.f, 1.f, 0.f, 1.f));
	materialZ->setParameter("baseColorFactor", glm::vec4(0.f, 0.f, 1.f, 1.f));

	compX->setMaterial(materialX, 0);
	compY->setMaterial(materialY, 0);
	compZ->setMaterial(materialZ, 0);

	x->setParent(go);
	y->setParent(go);
	z->setParent(go);

	return go;*/
//	return nullptr;
//}