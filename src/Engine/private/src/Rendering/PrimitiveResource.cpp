//module;
//
//#include <cassert>
//
//#include <vector>
//#include <memory>
//
//
//module Engine.Rendering.PrimitiveResource;
//
//import Engine.Rendering.ResourceManager;
//
//import Engine.Mesh;
//
//import RenderLib.RAII;
//
//
//using namespace Reef::Rendering;
//
//
//namespace
//{
//
//template<typename T>
//std::tuple<RenderLib::RAII::BufferPtr, RenderLib::RAII::BufferViewPtr>
//createAttributeBuffer(RenderLib::Context& context, 
//					  const std::vector<T>& v,
//					  RenderLib::AttributeFormat format)
//{
//	RenderLib::BufferConfig bufferConfig{};
//	bufferConfig.data	= v.data();
//	bufferConfig.size	= v.size() * sizeof(T);
//	bufferConfig.type	= RenderLib::BufferType::VERTEX_BUFFER;
//	auto buffer			= RenderLib::RAII::createBuffer(&context, bufferConfig);
//
//	RenderLib::BufferViewConfig config{};
//	config.buffer		= buffer.get();
//	config.numElements	= v.size();
//	config.offset		= 0;
//	config.attribute	= format;
//	auto view			= RenderLib::RAII::createBufferView(&context, config);
//
//	return { std::move(buffer), std::move(view) };
//}
//
//
//std::tuple<RenderLib::RAII::BufferPtr, RenderLib::RAII::BufferViewPtr>
//createIndexBuffer(RenderLib::Context& context,
//				  const std::vector<std::byte>& bytes, Engine::IndexFormat format)
//{
//	auto indexFormat = [format]
//	{
//		switch (format)
//		{
//			case Engine::IndexFormat::UNSIGNED_BYTE:
//				return RenderLib::AttributeFormat::UINT8;
//			case Engine::IndexFormat::UNSIGNED_SHORT:
//				return RenderLib::AttributeFormat::UINT16;
//			case Engine::IndexFormat::UNSIGNED_INT:
//				return RenderLib::AttributeFormat::UINT32;
//			default:
//				assert(false);
//				return RenderLib::AttributeFormat::UINT32;
//		}
//	}();
//
//	RenderLib::BufferConfig bufferConfig{};
//	bufferConfig.data	= bytes.data();
//	bufferConfig.size	= bytes.size();
//	bufferConfig.type	= RenderLib::BufferType::INDEX_BUFFER;
//	auto buffer			= RenderLib::RAII::createBuffer(&context, bufferConfig);
//
//	RenderLib::BufferViewConfig config{};
//	config.buffer		= buffer.get();
//	config.numElements	= bytes.size() / RenderLib::sizeInBytes(indexFormat);
//	config.offset		= 0;
//	config.attribute	= indexFormat;
//	auto view			= RenderLib::RAII::createBufferView(&context, config);
//	
//	return { std::move(buffer), std::move(view) };
//}
//
//
//} // namespace
//
//
//RenderLib::BufferView*
//PrimitiveResource::indices()
//{
//	return mIndicesView.get();
//}
//
//
//RenderLib::BufferView*
//PrimitiveResource::positions()
//{
//	return mPositionsView.get();
//}
//
//
//RenderLib::BufferView*
//PrimitiveResource::normals()
//{
//	return mNormalsView.get();
//}
//
//
//RenderLib::BufferView*
//PrimitiveResource::tangents()
//{
//	return mTangentsView.get();
//}
//
//
//RenderLib::BufferView*
//PrimitiveResource::texCoords()
//{
//	return mTexCoordsView.get();
//}
//
//
//bool
//PrimitiveResource::set(const Engine::Primitive& primitive, ResourceManager& manager)
//{
//	auto& context = *manager.context();
//
//	std::tie(mIndexBuffer, mIndicesView)		= createIndexBuffer(context, primitive.indices(), primitive.indexFormat());
//	std::tie(mPositionBuffer, mPositionsView)	= createAttributeBuffer(context, primitive.positions(), RenderLib::AttributeFormat::VEC3);
//	std::tie(mNormalBuffer, mNormalsView)		= createAttributeBuffer(context, primitive.normals(), RenderLib::AttributeFormat::VEC3);
//	std::tie(mTexCoordBuffer, mTexCoordsView)	= createAttributeBuffer(context, primitive.texcoords(), RenderLib::AttributeFormat::VEC2);
//	std::tie(mTangentBuffer, mTangentsView)		= createAttributeBuffer(context, primitive.tangents(), RenderLib::AttributeFormat::VEC3);
//
//	return true;
//}
