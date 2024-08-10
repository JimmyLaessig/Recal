module;

#include <Coral/System.h>

#include <cstdint>

export module Coral.Context;


export namespace Coral
{

class CommandQueue;
class Surface;
struct SurfaceConfig;

enum class GraphicsAPI
{
	OPENGL,
	VULKAN
};


struct CORAL_API ContextConfig
{
	GraphicsAPI graphicsAPI{ GraphicsAPI::VULKAN };
};


class CORAL_API Context
{
public:

	virtual ~Context() = default;

	///
	virtual GraphicsAPI graphicsAPI() const = 0;

	///
	virtual Coral::CommandQueue* getGraphicsQueue() = 0;

	///
	virtual Coral::CommandQueue* getComputeQueue() = 0;

	///
	virtual Coral::CommandQueue* getTransferQueue() = 0;

};

/// Create a new context
CORAL_API Context* createContext(const ContextConfig& config);

/// Destroy the context
CORAL_API void destroy(Context* context);

} // namespace Coral