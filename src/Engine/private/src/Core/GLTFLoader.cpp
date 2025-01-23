#include <Engine/Core/GLTFLoader.hpp>

#include <Engine/Application/Application.hpp>
#include <Engine/Rendering/IRenderer.hpp>

#include <Engine/Core/Scene.hpp>
#include <Engine/Core/SceneObject.hpp>
#include <Engine/Core/Component.hpp>
#include <Engine/Core/Texture.hpp>
#include <Engine/Core/Mesh.hpp>

#include <Engine/Component/MeshComponent.hpp>

#include <Common/Visitor.hpp>

#include <glm/glm.hpp>
#include <glm/gtx/matrix_decompose.hpp>

#include <tiny_gltf.h>

#include <execution>
#include <filesystem>
#include <utility>
#include <ranges>
#include <unordered_set>


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


template<size_t L>
glm::vec<L, float>
convert(const std::vector<double>& v, const glm::vec<L, float>& defaultValue)
{
	glm::vec<L, float> vec = defaultValue;

	for (size_t i = 0; i < std::min(v.size(), L); ++i)
	{
		vec[i] = v[i];
	}

	return vec;
}


template<typename T> std::pair<int, int> getType() { return {}; }

template<> std::pair<int, int> getType<uint8_t>() { return { TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE, TINYGLTF_TYPE_SCALAR }; }
template<> std::pair<int, int> getType<uint16_t>() { return { TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT, TINYGLTF_TYPE_SCALAR }; }
template<> std::pair<int, int> getType<uint32_t>() { return { TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT, TINYGLTF_TYPE_SCALAR }; }

template<> std::pair<int, int> getType<int8_t>() { return { TINYGLTF_COMPONENT_TYPE_BYTE, TINYGLTF_TYPE_SCALAR }; }
template<> std::pair<int, int> getType<int16_t>() { return { TINYGLTF_COMPONENT_TYPE_SHORT, TINYGLTF_TYPE_SCALAR }; }
template<> std::pair<int, int> getType<int32_t>() { return { TINYGLTF_COMPONENT_TYPE_INT, TINYGLTF_TYPE_SCALAR }; }

template<> std::pair<int, int> getType<float>() { return { TINYGLTF_COMPONENT_TYPE_FLOAT, TINYGLTF_TYPE_SCALAR }; }
template<> std::pair<int, int> getType<double>() { return { TINYGLTF_COMPONENT_TYPE_DOUBLE, TINYGLTF_TYPE_SCALAR }; }

template<> std::pair<int, int> getType<glm::vec2>() { return { TINYGLTF_COMPONENT_TYPE_FLOAT, TINYGLTF_TYPE_VEC2 }; }
template<> std::pair<int, int> getType<glm::vec3>() { return { TINYGLTF_COMPONENT_TYPE_FLOAT, TINYGLTF_TYPE_VEC3 }; }
template<> std::pair<int, int> getType<glm::vec4>() { return { TINYGLTF_COMPONENT_TYPE_FLOAT, TINYGLTF_TYPE_VEC4 }; }


template<typename T>
std::vector<T>
getAttribute(const tinygltf::Accessor& accessor, const tinygltf::Model& model)
{
	const auto& bufferView = model.bufferViews[accessor.bufferView];
	const auto& buffer = model.buffers[bufferView.buffer];

	auto [componentType, type] = getType<T>();


	auto dataPtr = buffer.data.data() + bufferView.byteOffset + accessor.byteOffset;

	std::vector<T> attribute(accessor.count);

	auto byteSize = tinygltf::GetComponentSizeInBytes(componentType) * tinygltf::GetNumComponentsInType(type);

	assert(accessor.type == type);
	assert(accessor.componentType == componentType);
	assert(byteSize == sizeof(T));

	// The attributes are tightly packed
	if (bufferView.byteStride == 0 || bufferView.byteStride == sizeof(T))
	{
		std::memcpy(attribute.data(), dataPtr, attribute.size() * byteSize);
	}
	// The attributes are in an interleaved buffer und must be deinterleaved
	else
	{
		for (size_t i = 0; i < accessor.count; ++i)
		{
			std::memcpy(&attribute[i], dataPtr + i * bufferView.byteStride, byteSize);
		}
	}

	return attribute;
}


bool
createPrimitive(const tinygltf::Primitive& gltfPrimitive, 
				const tinygltf::Model& model, Reef::Primitive& primitive, 
				const std::vector<std::shared_ptr<Reef::Material>>& materials)
{
	std::variant<std::vector<uint32_t>, std::vector<uint16_t>> indices;
	std::vector<glm::vec3> positions;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec3> tangents;
	std::vector<glm::vec2> texcoords;

	auto attribute = gltfPrimitive.attributes.find("POSITION");
	if (attribute != gltfPrimitive.attributes.end())
	{
		positions = getAttribute<glm::vec3>(model.accessors[attribute->second], model);
	}

	attribute = gltfPrimitive.attributes.find("NORMAL");
	if (attribute != gltfPrimitive.attributes.end())
	{
		normals = getAttribute<glm::vec3>(model.accessors[attribute->second], model);
	}

	attribute = gltfPrimitive.attributes.find("TEXCOORD_0");
	if (attribute != gltfPrimitive.attributes.end())
	{
		texcoords = getAttribute<glm::vec2>(model.accessors[attribute->second], model);
	}

	attribute = gltfPrimitive.attributes.find("TANGENT");
	if (attribute != gltfPrimitive.attributes.end())
	{
		//tangentAttribute = getAttribute<glm::vec4>(model.accessors[tangent->second], model);
	}

	if (gltfPrimitive.indices >= 0)
	{
		auto& accessor = model.accessors[gltfPrimitive.indices];

		switch (accessor.componentType)
		{
			case TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE:
			{
				auto indices8 = getAttribute<uint8_t>(accessor, model);
				indices = std::vector<uint16_t>(indices8.begin(), indices8.end());
				break;
			}
			case TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT:
			{
				indices = getAttribute<uint16_t>(accessor, model);
				break;
			}
			case TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT:
			{
				indices = getAttribute<uint32_t>(accessor, model);
				break;
			}
			default:
				break;
		}
	}

	auto success = Common::visit(indices, Common::Visitor{
		[&](const auto& i) { return primitive.set(i, positions, normals, tangents, texcoords); }
		});
	
	if (!success)
	{
		return false;
	}

	primitive.setMaterial(materials[static_cast<size_t>(gltfPrimitive.material)]);
	return true;
}


std::optional<TextureFormat>
getTextureFormat(const tinygltf::Image& image)
{
	auto pixelCount = image.width * image.height;
	auto componentSize = image.bits / 8;

	auto channelCount = image.image.size() / pixelCount / componentSize;

	std::pair<int, size_t> pixelFormat{ image.pixel_type, channelCount };

	if (pixelFormat == std::pair<int, int>{ TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE, 1 })
	{
		return TextureFormat::R_8U;
	}

	if (pixelFormat == std::pair<int, int>{ TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE, 2 })
	{
		return TextureFormat::RG_8U;
	}

	if (pixelFormat == std::pair<int, int>{ TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE, 3 })
	{
		return TextureFormat::RGB_8U;
	}

	if (pixelFormat == std::pair<int, int>{ TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE, 4 })
	{
		return TextureFormat::RGBA_8U;
	}

	if (pixelFormat == std::pair<int, int>{ TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT, 1 })
	{
		return TextureFormat::R_8U;
	}

	if (pixelFormat == std::pair<int, int>{ TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT, 2 })
	{
		return TextureFormat::RG_8U;
	}

	if (pixelFormat == std::pair<int, int>{ TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT, 3 })
	{
		return TextureFormat::RGB_8U;
	}

	if (pixelFormat == std::pair<int, int>{ TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT, 4 })
	{
		return TextureFormat::RGBA_8U;
	}


	assert(false && "Unsupported texture format");

	return {};
}


std::shared_ptr<Reef::Texture>
createTexture(const tinygltf::Texture& texture, const tinygltf::Model& model)
{
	const auto& image = model.images[texture.source];

	auto textureFormat = getTextureFormat(image);

	if (!textureFormat)
	{
		return nullptr;
	}

	return std::make_shared<Reef::Texture>(static_cast<uint32_t>(image.width), static_cast<uint32_t>(image.height),
										   *textureFormat, std::as_bytes(std::span{ image.image }), texture.name);
}


std::shared_ptr<Reef::Material>
createMaterial(const tinygltf::Material& material, 
			   const tinygltf::Model& model, 
			   const std::vector<std::shared_ptr<Texture>>& textures, 
			   std::shared_ptr<Reef::Rendering::Shader> shader)
{
	auto result = std::make_shared<Reef::Material>();
	result->setShader(shader);
	result->setName(material.name);

	auto getTextureOrDefault = [&](int index, std::shared_ptr<Reef::Texture> defaultTexture) { return index >= 0 ? textures[static_cast<size_t>(index)] : defaultTexture; };

	// PBR parameters
	result->setParameter("baseColor",				 convert(material.pbrMetallicRoughness.baseColorFactor, glm::vec4{ 1.f, 1.f, 1.f, 1.f }));
	result->setParameter("baseColorTexture",		 getTextureOrDefault(material.pbrMetallicRoughness.baseColorTexture.index, Reef::Texture::White));
	result->setParameter("roughness",				 static_cast<float>(material.pbrMetallicRoughness.roughnessFactor));
	result->setParameter("metallic",				 static_cast<float>(material.pbrMetallicRoughness.metallicFactor));
	result->setParameter("metallicRoughnessTexture", getTextureOrDefault(material.pbrMetallicRoughness.metallicRoughnessTexture.index, Reef::Texture::White));
	// Emissive
	result->setParameter("emissiveColor",			 convert(material.emissiveFactor, glm::vec3{ 0.f, 0.f, 0.f }));
	result->setParameter("emissiveTexture",			 getTextureOrDefault(material.pbrMetallicRoughness.metallicRoughnessTexture.index, Reef::Texture::White));
	// Occlusion
	result->setParameter("occlusionTexture",		 getTextureOrDefault(material.occlusionTexture.index, Reef::Texture::White));
	// Normal mapping
	result->setParameter("normalTexture",			 getTextureOrDefault(material.normalTexture.index, Reef::Texture::Normal));

	return result;
}


std::shared_ptr<Reef::Mesh>
createMesh(const tinygltf::Mesh& gltfMesh, const tinygltf::Model& model, const std::vector<std::shared_ptr<Material>>& materials)
{
	auto result = std::make_shared<Reef::Mesh>();

	for (auto& gltfPrimitive : gltfMesh.primitives)
	{
		if (!createPrimitive(gltfPrimitive, model, result->addPrimitive(), materials))
		{
			return nullptr;
		}
	}

	return result;
}


void
createSceneGraphRecursive(const tinygltf::Node& node, const tinygltf::Model& model, SceneObject* parent, Scene& scene,
	const std::vector<std::shared_ptr<Mesh>>& meshes)
{
	auto go = scene.createSceneObject(node.name);
	go->setParent(parent);

	if (node.mesh >= 0)
	{
		auto meshComponent = go->addComponent<MeshComponent>();
		meshComponent->setMesh(meshes.at(static_cast<size_t>(node.mesh)));
	}

	auto trs = getTransform(node);
	go->transform().setLocalPose(trs.translation, trs.rotation, trs.scale);

	for (auto nodeIndex : node.children)
	{
		createSceneGraphRecursive(model.nodes[nodeIndex], model, go, scene, meshes);
	}
}


void
createSceneGraph(const tinygltf::Scene& gltfScene, const tinygltf::Model& model, SceneObject* parent, Scene& scene,
	const std::vector<std::shared_ptr<Mesh>>& meshes)
{
	auto go = scene.createSceneObject(gltfScene.name);
	go->setParent(parent);

	for (auto nodeIndex : gltfScene.nodes)
	{
		createSceneGraphRecursive(model.nodes[nodeIndex], model, go, scene, meshes);
	}
}


SceneObject*
loadModel(Scene& scene, const tinygltf::Model& model, const std::optional<size_t>& sceneIndex)
{
	auto shader = scene.application().renderer().getShader(Reef::Rendering::BuiltInShaders::DefaultPBR);

	if (sceneIndex && sceneIndex >= model.scenes.size())
	{
		return nullptr;
	}

	std::vector<size_t> scenes;
	if (sceneIndex)
	{
		scenes.push_back(*sceneIndex);
	}
	else
	{
		scenes.resize(model.scenes.size());
		std::iota(scenes.begin(), scenes.end(), 0);
	}

	auto textures = model.textures |
		std::views::transform([&](const auto& texture) { return createTexture(texture, model); }) |
		std::ranges::to<std::vector>();

	auto materials = model.materials |
		std::views::transform([&](const auto& material) { return createMaterial(material, model, textures, shader); }) |
		std::ranges::to<std::vector>();

	auto meshes = model.meshes |
		std::views::transform([&](const auto& mesh) { return createMesh(mesh, model, materials); }) |
		std::ranges::to<std::vector>();

	auto root = scene.createSceneObject("model");

	// Finally, create the scene graph
	for (auto sceneIndex : scenes)
	{
		createSceneGraph(model.scenes[sceneIndex], model, root, scene, meshes);
	}

	return root;
}

} // namespace


SceneObject*
GLTFLoader::load(Scene& scene, const std::filesystem::path& path)
{
	tinygltf::Model model;
	std::string err;
	std::string warn;
	tinygltf::TinyGLTF loader;

	auto extension = path.extension();
	if (extension == ".glb")
	{
		if (!loader.LoadBinaryFromFile(&model, &err, &warn, path.string()))
		{
			return nullptr;
		}
	}
	else if (extension == ".gltf")
	{
		if (!loader.LoadASCIIFromFile(&model, &err, &warn, path.string()))
		{
			return nullptr;
		}
	}
	else
	{
		return nullptr;
	}

	return loadModel(scene, model, {});
}




















//GameObject*
//GLTFImporter::createSkybox(Scene& scene, std::shared_ptr<Engine::Texture> skyboxTexture)
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
//GLTFImporter::createCoordinateCross(Scene& scene, float scale)
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