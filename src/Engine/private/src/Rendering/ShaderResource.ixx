//export module Engine.Rendering.ShaderResource;
//
//import Engine.Rendering.RenderResource;
//
//import Engine.Shader;
//
//import RenderLib.RAII;
//
//namespace Reef
//{
//
//namespace Rendering
//{
//
//export class PerViewResource
//{
//public:
//
//	PerViewResource(Engine::Shader& shader);
//
//	RenderLib::Shader* shader();
//
//protected:
//
//	bool updateInternal(RenderLib::Context& context) override;
//
//private:
//
//	Engine::Shader& mShader;
//
//	RenderLib::RAII::ShaderPtr mGPUShader;
//};
//
//} // Rendering
//
//} // Engine
//
