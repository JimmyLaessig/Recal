//export module RenderLib.OpenGL.Texture2D;
//
//import RenderLib.Texture2D;
//
//import RenderLib.OpenGL.Context;
//
//namespace RenderLib
//{
//namespace OpenGL
//{
//	 
//export class Texture2DImpl : public RenderLib::Texture2D
//{
//public:
//
//	bool init(Context* context, const Texture2DConfig& config);
//
//	~Texture2DImpl();
//
//	bool hasMipmaps() const { return mHasMipmaps; }
//
//	uint32_t handle() const { return mHandle; }
//
//	Type type() const { return Type::_2D; }
//
//	PixelFormat pixelFormat() const override { return mFormat; }
//
//	uint32_t width() const override { return mWidth; }
//
//	uint32_t height() const override { return mHeight; }
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
//	/// The width of the texture (must be greater than 0)
//	uint32_t mWidth{ 0 };
//	
//	/// The height of the texture (must be greater than 0)
//	uint32_t mHeight{ 0 };
//
//	/// Flag indicating if the texture has mip maps allocated
//	bool mHasMipmaps{ false };
//
//	/// The native OpenGL handle of the texture
//	uint32_t mHandle{ 0 };
//};
//
//} // namespace OpenGL
//
//} // namespace RenderLib