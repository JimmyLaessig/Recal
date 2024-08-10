//module;
//
//#include <memory>
//
//export module RenderLib.OpenGL.Texture3D;
//
//import RenderLib.Texture3D;
//import RenderLib.Context;
//import RenderLib.OpenGL.Context;
//
//namespace RenderLib
//{
//namespace OpenGL
//{
//	 
//export class Texture3DImpl : public RenderLib::Texture3D
//{
//public:
//
//	bool init(Context* mContext, const Texture3DConfig& config);
//
//	~Texture3DImpl();
//
//	bool hasMipmaps() const { return mHasMipmaps; }
//
//	uint32_t handle() const { return mHandle; }
//
//	Type type() const { return Type::_3D; }
//
//	PixelFormat pixelFormat() const override { return mFormat; }
//
//	uint32_t width() const override { return mWidth; }
//
//	uint32_t height() const override { return mHeight; }
//
//	uint32_t depth() const override { return mDepth; }
//
//	bool hasMips() const override { return mHasMipmaps; };
//
//private:
//
//	/// The OpenGL context
//	Context* mContext;
//
//	/// The Pixel Format of the texture
//	PixelFormat mFormat{ PixelFormat::RGBA8_UI };
//	
//	/// The width of the texture
//	uint32_t mWidth{ 0 };
//	
//	/// The height of the texture
//	uint32_t mHeight{ 0 };
//
//	/// The height of the texture
//	uint32_t mDepth{ 0 };
//
//	/// Flag indicating if the texture has mip maps allocated
//	bool mHasMipmaps{ false };
//
//	/// The native OpenGL handle of the texture
//	uint32_t mHandle{ 0 };
//};
//
//} // namespace OpenGL
//} // namespace RenderLib