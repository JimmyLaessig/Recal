module;

#include <Coral/System.h>

#include <cstdint>

export module Coral.BufferView;

import Coral.Types;

export namespace Coral
{

class Buffer;
class Context;

/// Configuration to create a BufferView
 struct BufferViewConfig
{
	/// The buffer on which to create a view
	/**
	 * Buffer View creation fails if
	 * buffer is nullptr.
	 */
	Buffer* buffer{ nullptr };

	/// The count of elements in the buffer view
	/**
	 * Buffer View creation fails if
	 * * numElements is 0
	 * * numElements * sizeInBytes(attribute) + offset exceed the buffer's size.
	 */
	uint32_t numElements{ 0 };

	/// The offset from the base address of the buffer
	/**
	 * Buffer View creation fails if
	 * * count * sizeInBytes(attribute) + offset exceed the buffer's size.
	 */
	uint32_t offset{ 0 };

	/// The stride between elements in bytes. If zero, the buffer view assumes the elements are tightly packed and the stride will be set to the byte size of one element.
	 uint32_t stride{ 0 };

	/// The format of the attribute that this view is accessing. 
	AttributeFormat attribute{ AttributeFormat::VEC4 };
};


/// A BufferView provides the GPU structured access to a Buffer's data in the shader
class BufferView
{
public:

	virtual ~BufferView() = default;

	/// Get the offset from the base address of the buffer
	virtual uint32_t offset() const = 0;

	/// Get the stride
	virtual uint32_t stride() const = 0;

	/// Get the number of elements contained in the view
	virtual uint32_t numElements() const = 0;

	/// Get the format of the attribute
	virtual AttributeFormat attributeFormat() const = 0;

	/// Get the buffer underneath the view
	virtual const Buffer* buffer() const = 0;

	/// Get the buffer underneath the view
	virtual Buffer* buffer() = 0;
};

/// Create a new buffer view
CORAL_API BufferView* createBufferView(Context* context, const BufferViewConfig& config);

/// Destroy the buffer view
CORAL_API void destroy(BufferView* buffer);

} // namespace Coral
