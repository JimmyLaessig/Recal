//module;
//
//#include <memory>
//#include <vector>

//export module Engine.Rendering.SceneRenderer;
//
//import Engine.Rendering.GBuffer;
//import Engine.Rendering.GeometryRenderer;
//import Engine.Rendering.SceneView;
//import Engine.Rendering.FXAATechnique;
//import Engine.Rendering.ToneMappingTechnique;
//import Engine.Rendering.GammaCorrectionTechnique;
//import Engine.Rendering.LightingPass;
//import Engine.Rendering.Renderer;
//
//import Engine.Camera;
//import Engine.Material;
//import Engine.Mesh;
//import Engine.Scene;
//
//import Singleton;
//
//import RenderLib.RAII;
//
//
//namespace Reef
//{
//
//namespace Rendering
//{
//
//export class SceneRenderer
//{
//public:
//
//	bool initialize(RenderLib::Context& context,
//					uint32_t width, uint32_t height,
//					RenderLib::Framebuffer& framebuffer);
//
//	void setSceneView(const SceneView& view);
//
//	RenderLib::CommandBuffer* recordRenderCommands();
//
//	GBuffer& gBuffer();
//
//private:
//
//	void updateRenderResource(SceneView& view);
//
//	RenderLib::Context* mContext{ nullptr };
//
//	RenderLib::Framebuffer* mFramebuffer{ nullptr };
//
//	SceneView mSceneView;
//
//	GeometryRenderer mGeometryRenderer;
//
//	LightingPass mLightingPass;
//
//	ToneMappingTechnique mTonemappingTechnique;
//
//	GammaCorrectionTechnique mGammaCorrectionTechnique;
//
//	FXAATechnique mFXAATechnique;
//
//	RenderLib::RAII::CommandBufferPtr mCommandBuffer;
//
//};

//} // namespace Rendering
//
//} // namespace Reef