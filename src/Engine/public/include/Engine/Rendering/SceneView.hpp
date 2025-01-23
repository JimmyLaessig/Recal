#ifndef ENGINE_RENDERING_SCENEVIEW_HPP
#define ENGINE_RENDERING_SCENEVIEW_HPP

#include <Engine/Core/Primitive.hpp>
#include <Engine/Core/Camera.hpp>

#include <glm/glm.hpp>

#include <memory>
#include <vector>

namespace Reef::Rendering
{

struct TransformData
{
	glm::mat4 localToWorldMatrix;

	glm::mat4 worldToLocalMatrix;

	glm::mat3 normalMatrix;

	size_t objectId;
};


struct ENGINE_API RenderObject
{
	/// The transform data of the render object
	TransformData transform;

	/// The primitive of the render object
	std::shared_ptr<Reef::Primitive> primitive;

	/// The material of the render object
	std::shared_ptr<Reef::Material> material;
};


struct ENGINE_API SceneView
{	
	/// The camera through which to view the scene
	Reef::Camera camera;
	/// View matrix of the camera
	glm::mat4 viewMatrix;
	/// World-space position of the camera
	glm::vec3 cameraPosition;
	/// View-projection matrix of the camera
	glm::mat4 viewProjectionMatrix;
	/// Inverse view-projection matrix of the camera
	glm::mat4 inverseViewProjectionMatrix;
	/// The 3D objects in the scene
	std::vector<RenderObject> renderObjects;
};

} // namespace Reef

#endif // !ENGINE_RENDERING_SCENEVIEW_HPP
