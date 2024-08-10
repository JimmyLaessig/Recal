//module;
//
//#include <string_view>
//#include <memory>
//
//module Engine.Shader;
//
//import Engine.Texture;
//import Engine.Rendering.ShaderManager;
//
//using namespace Reef;
//
//
//Shader::Shader(std::string_view name, RenderLib::RAII::ShaderPtr shader)
//	: mName(name)
//	, mShader(std::move(shader))
//{
//	
//}
//
//
//const std::string&
//Shader::name() const
//{
//	return mName;
//}
//
//
//RenderLib::Shader*
//Shader::shader()
//{
//	return mShader.get();
//}