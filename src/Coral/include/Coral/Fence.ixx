module;

#include <Coral/System.h>

export module Coral.Fence;

export namespace Coral
{

class Context;

// A fence is a GPU-CPU synchronization mechanic
class CORAL_API Fence
{
public:

	virtual ~Fence() = default;

	virtual bool wait() = 0;

	virtual void reset() = 0;
}; // class Fence

/// Create a new Fence object
Fence* createFence(Context* context);

/// Destroy the Fence object
CORAL_API void destroy(Fence* fence);


} // namespace Coral