module;

#include <Coral/System.h>

#include <cstddef>

export module Coral.Buffer;

import Coral.Types;

export namespace Coral
{

export class Context;

/// The type of the buffer
export enum class BufferType
{
	VERTEX_BUFFER,
	INDEX_BUFFER,
	UNIFORM_BUFFER,
	STORAGE_BUFFER
};

/// Configuration to create a Buffer
export struct BufferConfig
{
	/// The size of the buffer in bytes
	/**
	 * Buffer creation fails if size is 0.
	 */
	size_t size{ 0 };

	/// The type of the buffer
	BufferType type{ BufferType::VERTEX_BUFFER };

	/// Flag indicating if the buffer's memory should can be mapped to CPU memory
	bool cpuVisible{ false };
};


class CORAL_API Buffer
{
public:

	virtual ~Buffer() = default;

	/// Get the size of the buffer in bytes
	virtual size_t size() const = 0;

	/// Get the type of the buffer
	virtual BufferType type() const = 0;

	/// Map the buffer memory to CPU-accessible memory.
	/**
	 * This function only succeeds if the buffer was created with the 
	 * 'cpuVisible' option enabled.
	 */
	virtual std::byte* map() = 0;

	/// Unmap the buffer memory, mapped previously by 'map()'.
	virtual bool unmap() = 0;
};

/// Create a new buffer
CORAL_API Buffer* createBuffer(Context* context, const BufferConfig& config);

/// Destroy the buffer
CORAL_API void destroy(Buffer* buffer);

} // namespace Coral