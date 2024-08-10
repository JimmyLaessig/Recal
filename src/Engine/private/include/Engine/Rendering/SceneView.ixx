module;

#include <glm/glm.hpp>

#include <memory>
#include <vector>
#include <variant>

export module Engine.Rendering.SceneView;

import Engine.Primitive;
import Engine.Material;
import Engine.Camera;
import Engine.Texture;
//import Engine.DirectionalLight;
//import Engine.SkyLight;

import Coral.Framebuffer;

namespace Reef
{

namespace Rendering
{

export struct RenderObject
{
	/// Transform the object from local to world space
	glm::mat4 localToWorldMatrix{ 1.f };

	/// Transform the object from world to local space
	glm::mat4 worldToLocalMatrix{ 1.f };

	/// Transform the normals to world space
	glm::mat3 normalMatrix{ 1.f };

	/// The primitive of the render object
	std::shared_ptr<Reef::Primitive> primitive;

	/// The material of the render object
	std::shared_ptr<Reef::Material> material;

	/// The unique id of the render object
	size_t uniqueId{ 0 };
};


export struct SceneView
{	
	glm::mat4 viewMatrix;

	glm::vec3 cameraPosition;

	//Reef::Camera camera;

	//std::shared_ptr<Coral::Framebuffer> framebuffer;

	std::vector<RenderObject> renderObjects;

	/*std::vector<std::shared_ptr<SkyLight>> skyLights;

	std::vector<std::shared_ptr<DirectionalLight>> directionalLights;*/

	glm::mat4 viewProjectionMatrix;

	glm::mat4 inverseViewProjectionMatrix;
};

} // export namespace Rendering

} // export namespace Reef