export module Engine.Rendering.PrimitiveResource;

//import Engine.Rendering.RenderResource;

import Engine.Primitive;

import Coral.RAII;


namespace Reef
{

namespace Rendering
{

export class ResourceManager;

export class PrimitiveResource
{
public:

	/*RenderLib::BufferView* indices();

	RenderLib::BufferView* positions();

	RenderLib::BufferView* normals();

	RenderLib::BufferView* tangents();

	RenderLib::BufferView* texCoords();*/

	bool set(const Reef::Primitive& primitive, ResourceManager& manager);

private:

	//RenderLib::RAII::BufferPtr mIndexBuffer;
	//RenderLib::RAII::BufferPtr mPositionBuffer;
	//RenderLib::RAII::BufferPtr mNormalBuffer;
	//RenderLib::RAII::BufferPtr mTangentBuffer;
	//RenderLib::RAII::BufferPtr mTexCoordBuffer;

	//RenderLib::RAII::BufferViewPtr mIndicesView;
	//RenderLib::RAII::BufferViewPtr mPositionsView;
	//RenderLib::RAII::BufferViewPtr mNormalsView;
	//RenderLib::RAII::BufferViewPtr mTangentsView;
	//RenderLib::RAII::BufferViewPtr mTexCoordsView;
};

} // Rendering

} // Engine
