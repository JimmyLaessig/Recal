module;

#include <condition_variable>
#include <mutex>
#include <string_view>
#include <thread>
#include <queue>
#include <functional>
#include <future>

module AsyncTaskQueue;


AsyncTaskQueue::AsyncTaskQueue()
	:mThread([this] { run(); })
{
}


AsyncTaskQueue::~AsyncTaskQueue()
{
	flush();

	{
		std::lock_guard lock(mQueueProtection);
		mExit = true;
	}

	mConditionalVariable.notify_all();

	mThread.join();
}


void
AsyncTaskQueue::enqueue(AsyncTaskQueue::Task&& task)
{
	std::lock_guard lock(mQueueProtection);
	mQueue.push(std::forward<Task&&>(task));
	mConditionalVariable.notify_one();
}


void 
AsyncTaskQueue::flush()
{
	std::promise<void> p;
	std::future<void> f = p.get_future();

	enqueue([p = &p]
	{ 
		p->set_value();
	});
	
	f.get();
}


void
AsyncTaskQueue::run()
{
	std::unique_lock lock(mQueueProtection);

	while (true)
	{
		// Wait until tasks have been queued for execution
		mConditionalVariable.wait(lock, [this]
			{ return !mQueue.empty() || mExit; });

		// The destructor notifies the runner thread to exit
		if (mExit)
		{
			return;
		}

		auto task = std::move(mQueue.front());
		mQueue.pop();
		lock.unlock();

		// Execute the task after unlocking the mutex to not block other
		// threads from enqueuing new tasks
		task();

		//The condition variable's wait function requires a lock. If  the 
		// thread is waiting for data, it will release the mutex and only 
		// re-lock when notified and ready to run. This is why we lock at
		// the end of the while loop.
		lock.lock();
	}
}
