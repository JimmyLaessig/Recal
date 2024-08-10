module;

#include <Coral/System.h>

#include <span>

export module Coral.CommandQueue;

export namespace Coral
{

class CommandBuffer;
class Semaphore;
class Fence;
class Surface;

struct CORAL_API CommandBufferSubmitInfo
{
	/// The command buffers to to execute in batch. 
	/**
	 * The order of command buffers in the list dictates the order of
	 * submission and beginning of execution, but are allowed to proceed
	 * independently after that and complete out of order.
	 */
	std::span<CommandBuffer*> commandBuffers;

	std::span<Semaphore*> waitSemaphores;

	std::span<Semaphore*> signalSemaphores;

}; // struct SubmitInfo


struct PresentInfo
{
	/// List of command buffers this submission depends on. 
	/**
	 * Presentation will start once all dependency command buffers have
	 * finished execution.
	 *
	 * \note: The caller must ensure that the dependency command buffers
	 * are submitted for execution to any queue prior to submitting them as
	 * a dependency.
	 */
	std::span<Semaphore*> waitSemaphores;

	Surface* surface{ nullptr };
}; // struct PresentInfo


/// A Command Queue executes submitted work
class CORAL_API CommandQueue
{
public:

	virtual ~CommandQueue() = default;

	/// Submit a sequence of command buffers to a queue
	virtual bool submit(const CommandBufferSubmitInfo& info, Fence* fence) = 0;
	
	/// Wait until the queue is idle.
	virtual bool waitIdle() = 0;

	/// Present the last rendered image to the screen.
	virtual bool submit(const PresentInfo& info) = 0;

}; // class CommandQueue

} // namespace Coral
