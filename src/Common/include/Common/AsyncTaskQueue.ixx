module;

#include <Common/System.h>

#include <condition_variable>
#include <functional>
#include <mutex>
#include <queue>
#include <string_view>
#include <thread>

export module Common.AsyncTaskQueue;

import Common.NonCopyable;

export namespace Common
{

/// The AsnycTaskQueue executes work on a separate worker thread
export class COMMON_API AsyncTaskQueue : public Common::NonCopyable
{
public:

	using Task = std::function<void()>;

	/// Create a new asynchronuous task queue with a named thread
	AsyncTaskQueue(std::string_view threadName);

	/// Create a new asynchronuous task queue
	AsyncTaskQueue();

	/// Destructor
	~AsyncTaskQueue();

	/// Enqueue a new task to the queue
	void enqueue(Task&& task);

	/// Blocks until all queued tasks are executed
	void flush();

private:

	/// Runner function that is executed on the worker thread
	void run();

	/// Flag indicating if the worker thread should stop. This is only set to true from within the destructor
	bool mStop{ false };

	/// Conditional variable to notify the worker thread that new tasks were enqueued
	std::condition_variable mConditionalVariable;

	/// Control access of the shared resources between worker thread and caller thread. 
	std::mutex mQueueProtection;

	/// Queue that holds all tasks to be executed
	std::queue<Task> mQueue;

	/// Worker thread of the async task queue
	std::thread mThread;
};

} // namespace Common