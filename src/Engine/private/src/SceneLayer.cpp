module;

#include <Engine/System.h>

#include <glm/glm.hpp>

#include <memory>
#include <mutex>
#include <vector>

module Engine.Application.EngineLayer;
import Engine.Application.WindowLayer;
import Engine.Application;

//
//import Engine.Rendering.Renderer;
//

//import Engine.Application;
//import Engine.GameObject;
//
//import Engine.Component.CameraComponent;
//import Engine.Component.DirectionalLightComponent;
//import Engine.Component.MeshComponent;
//import Engine.Component.SkyLightComponent;
//
//import Engine.Camera;
//import Engine.DirectionalLight;
//import Engine.SkyLight;
//
using namespace Reef;

namespace
{
//
//template <class T>
//inline void hashCombine(std::size_t& seed, const T& v)
//{
//	std::hash<T> hasher;
//	seed ^= hasher(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
//}
//
//
//void
//addRenderObjects(Engine::GameObject& gameObject, 
//				 const glm::mat4& viewMatrix,
//				 const glm::mat4& projectionMatrix,
//				 const glm::mat4& viewProjectionMatrix, 
//				 std::vector<Engine::Rendering::RenderObject>& results)
//{
//	auto localToWorlMatrix = gameObject.transform().localToWorldMatrix();
//	auto worldToLocalMatrix = gameObject.transform().worldToLocalMatrix();
//	glm::mat3 normalMatrix = glm::inverse(glm::transpose(localToWorlMatrix));
//
//	for (auto mc : gameObject.getComponents<Engine::MeshComponent>())
//	{
//		if (auto mesh = mc->mesh())
//		{
//			for (uint32_t i = 0; i < mesh->getNumPrimitives(); ++i)
//			{
//				auto& ro		= results.emplace_back();
//				ro.primitive	= mesh->getPrimitive(i);
//				ro.material		= ro.primitive->material();
//				if (auto material = mc->material(i))
//				{
//					ro.material = material;
//				}
//				ro.localToWorldMatrix = localToWorlMatrix;
//				ro.worldToLocalMatrix = worldToLocalMatrix;
//				ro.normalMatrix		  = normalMatrix;
//				ro.uniqueId			  = 0;
//				hashCombine(ro.uniqueId, i);
//				hashCombine(ro.uniqueId, mc->runtimeUniqueId());
//			}
//		}
//	}
//
//	for (auto child : gameObject.children())
//	{
//		addRenderObjects(*child, viewMatrix, projectionMatrix, viewProjectionMatrix, results);
//	}
//}

} // namespace


bool
SceneLayer::initialize()
{
	/*mRenderer.reset();

	auto& renderer = Engine::Rendering::Renderer::instance();*/

	if (auto windowLayer = application().getLayer<WindowLayer>())
	{
		//auto surface = windowLayer->renderSurface();

		//if (!surface)
		//{
		//	return false;
		//}

		/*auto backbuffer = surface->backbuffer();

		if (!backbuffer)
		{
			return false;
		}
		
		auto sceneRenderer = std::make_unique<Engine::Rendering::SceneRenderer>();

		if (!sceneRenderer->initialize(renderer.context(), 2048, 1024, *backbuffer))
		{
			return false;
		}

		mRenderer = std::move(sceneRenderer);*/

		//auto debugRenderer = std::make_unique<Engine::Rendering::DebugRenderer>();

		//if (!debugRenderer->initialize(renderer.context(), 2048, 1024, backbuffer))
		//{
		//	return false;
		//}

		//mDebugRenderer = std::move(debugRenderer);
	}

	return true;
}


void
SceneLayer::start()
{

}


void 
SceneLayer::stop()
{
}


void
SceneLayer::update()
{
	//mScene.update(0.f);

	//resolveScene();
}


//void
//SceneLayer::render()
//{
//	//std::shared_ptr<Engine::Rendering::SceneView> sceneView;
//	//{
//	//	std::lock_guard lock(mResolvedSceneViewProtection);
//
//	//	sceneView = mResolvedSceneView;
//	//}
//
//	//if (!sceneView)
//	//{
//	//	// TODO: Log error
//	//	return;
//	//}
//
//	//auto& renderer = Engine::Rendering::Renderer::instance();
//
//	//if (mRenderer)
//	//{
//	//	mRenderer->setSceneView(*sceneView);
//	//	renderer.submitCommandBuffer(mRenderer->recordRenderCommands());
//	//}
//
//	//if (mDebugRenderer)
//	//{
//	//	std::vector<Engine::MeshComponent*> v;
//
//	//	for (GameObject* go : mScene.gameObjects())
//	//	{
//	//		if (go->name() == "Sphere")
//	//		{
//	//			if (Engine::MeshComponent* mesh = go->getComponent<Engine::MeshComponent>())
//	//			{
//	//				v.push_back(mesh);
//
//	//			}
//	//		}
//	//	}
//	//	mDebugRenderer->setSelection(v);
//
//	//	mDebugRenderer->setSceneView(*sceneView);
//	//	renderer.submitCommandBuffer(mDebugRenderer->recordRenderCommands());
//	//}
//}


//void
//SceneLayer::render()
//{
//	
//}


//void
//SceneLayer::render()
//{
//	auto& renderer = Engine::Rendering::Renderer::instance();
//
//	renderer.execute();
//}


Scene&
SceneLayer::scene()
{
	return mScene;
}


const Scene&
SceneLayer::scene() const
{
	return mScene;
}


//void
//SceneLayer::resolveScene()
//{
//	auto view = std::make_shared<::Engine::Rendering::SceneView>();
//
//	auto cameraComponent = mScene.findComponent<Engine::CameraComponent>();
//
//	if (!cameraComponent)
//	{
//		return;
//	}
//
//	view->viewMatrix		= cameraComponent->viewMatrix();
//	view->camera			= cameraComponent->camera();
//	view->cameraPosition	= cameraComponent->gameObject().transform().worldPosition();
//
//	view->viewProjectionMatrix			= view->camera.projectionMatrix * view->viewMatrix;
//	view->inverseViewProjectionMatrix	= glm::inverse(view->viewProjectionMatrix);
//
//	for (auto light : mScene.findComponents<Engine::DirectionalLightComponent>())
//	{
//		view->directionalLights.push_back(std::static_pointer_cast<DirectionalLight>(light->light()));
//	}
//
//	for (auto light : mScene.findComponents<Engine::SkyLightComponent>())
//	{
//		view->skyLights.push_back(std::static_pointer_cast<SkyLight>(light->light()));
//	}
//
//	// Traverse the scene graph to collect all meshes
//	// This performs view-frustum culling
//	for (auto child : mScene.gameObjects())
//	{
//		addRenderObjects(*child, view->viewMatrix, view->camera.projectionMatrix, view->viewProjectionMatrix, view->renderObjects);
//	}
//
//	// The mResolvedSceneView will get picked up by the render thread. Therefore, 
//	// we need to protect it for the duration of this function. This function
//	// must be implemented very performant as the render thread will wait for 
//	// the scene to be resolved
//	std::lock_guard lock(mResolvedSceneViewProtection);
//	mResolvedSceneView = view;
//}
