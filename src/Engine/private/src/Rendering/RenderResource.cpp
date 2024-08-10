module;

#include <glm/glm.hpp>

module Engine.Rendering.RenderResource;

import RenderLib;
import Engine.Rendering.ResourceManager;
import Engine.Rendering.UniformBufferFactory;

import Visitor;

using namespace Reef::Rendering;

bool
RenderResource::set(const RenderObject& renderObject, const SceneView& view, ResourceManager& resourceManager)
{
	auto context					= resourceManager.context();
	auto modelViewMatrix			= view.viewMatrix * renderObject.localToWorldMatrix;
	auto modelViewProjectionMatrix	= view.viewProjectionMatrix * modelViewMatrix;

	

	return true;
}
