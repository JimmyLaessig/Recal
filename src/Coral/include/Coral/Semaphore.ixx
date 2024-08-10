module;

#include <Coral/System.h>

export module Coral.Semaphore;

export namespace Coral
{

class Context;

class CORAL_API Semaphore
{
public:

	virtual ~Semaphore() = default;

}; // class Semaphore

/// Create a new Semaphore object
CORAL_API Semaphore* createSemaphore(Context* context);

/// Destroy the Semaphore object
CORAL_API void destroy(Semaphore* surface);

} // namespace Coral