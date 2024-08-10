//module Engine.Rendering.ShaderResource;
//
//using namespace Reef::Rendering;
//
//
//ShaderResource::ShaderResource(Engine::Shader& shader)
//	: mShader(shader)
//{
//}
//
//
//RenderLib::Shader*
//ShaderResource::shader()
//{
//	return mGPUShader.get();
//}
//
//
//bool
//ShaderResource::updateInternal(Engine::Rendering::)
//{
//	RenderLib::ShaderConfig config{};
//	config.name					= "UniformColor";
//	config.vertexShaderSource	= mShader.mVertexShaderSourceGLSL.c_str();
//	config.fragmentShaderSource = mShader.mFragmentShaderSourceGLSL.c_str();
//	mGPUShader					= RenderLib::RAII::createShader(&context, config);
//	
//	return mGPUShader != nullptr;
//}