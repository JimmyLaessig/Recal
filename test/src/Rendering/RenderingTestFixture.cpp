//#include "RenderingTestFixture.h"
//#include <glfw/glfw3.h>
//
//
//RenderingTestFixture::RenderingTestFixture(Rendering::GraphicsAPI backend)
//{
//	init(backend);
//}
//
//
//RenderingTestFixture::~RenderingTestFixture()
//{
//	mContext.reset();
//
//	if (mOffscreenContext)
//	{
//		glfwDestroyWindow(mOffscreenContext);
//		mOffscreenContext = nullptr;
//	}
//}
//
//
//Rendering::Context* 
//RenderingTestFixture::getContext()
//{
//	return mContext.get();
//}
//
//
//void
//RenderingTestFixture::init(Rendering::GraphicsAPI api)
//{
//	REQUIRE(glfwInit() == GLFW_TRUE);
//	glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
//	//glfwWindowHint(GLFW_COCOA_MENUBAR, GLFW_FALSE);
//
//#if _DEBUG
//	// Create a debug OpenGL context or tell your OpenGL library (GLFW, SDL) to do so.
//	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
//#endif
//
//	// Set window hints
//	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
//	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
//	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
//	glfwWindowHint(GLFW_REFRESH_RATE, 60);
//
//	mOffscreenContext = glfwCreateWindow(2048, 1024, "", nullptr, nullptr);
//	REQUIRE(mOffscreenContext);
//	glfwMakeContextCurrent(mOffscreenContext);
//	
//	Rendering::ContextConfig contextConfig{};
//	contextConfig.graphicsAPI = api;
//
//	mContext = Rendering::RAII::createContext(contextConfig);
//
//	REQUIRE(mContext);
//}