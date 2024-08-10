module;

module Coral.Vulkan.BufferView;

import Coral.Vulkan.Buffer;
import Coral.Vulkan.Context;

using namespace Coral::Vulkan;

bool
BufferView::init(Coral::Vulkan::Context* context, const Coral::BufferViewConfig& config)
{
	if (!context)
	{
		return false;
	}

	if (!config.buffer)
	{
		return false;
	}

	if (config.numElements == 0)
	{
		return false;
	}

	mStride = config.stride == 0 ? Coral::sizeInBytes(config.attribute) : config.stride;

	auto byteSize = config.numElements * mStride;

	if (byteSize + config.offset < config.buffer->size())
	{
		return false;
	}

	mBuffer				= static_cast<Coral::Vulkan::Buffer*>(config.buffer);
	mByteOffset			= config.offset;
	
	mAttributeFormat	= config.attribute;
	mNumElements		= config.numElements;

	return true;
}


uint32_t
BufferView::offset() const
{ 
	return mByteOffset;
}


uint32_t
BufferView::stride() const
{ 
	return mStride;
}


uint32_t
BufferView::numElements() const
{ 
	return mNumElements;
}


Coral::AttributeFormat
BufferView::attributeFormat() const
{ 
	return mAttributeFormat;
}


Coral::Buffer*
BufferView::buffer()
{ 
	return mBuffer;
}


const Coral::Buffer* BufferView::buffer() const
{ 
	return mBuffer;
}