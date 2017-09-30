
#ifndef threadpool_hpp
#define threadpool_hpp

/**
 * A pool of threads available to process tasks
 */
class GIGA_API ThreadPool {
public:
	ThreadPool();
	~ThreadPool() = default;

	/**
	 * Add a thread to the pool
	 */
	void AddThread(TaskThread* thread);

	/**
	 * Execute a task pool
	 */
	void Execute(TaskPool* pool);

	/**
	* Get next available task
	*/
	Task* GetNextTask();

	/**
	* Add a task to the current task pool
	*/
	void AddTask(Task* task);

protected:
	// Threads
	std::vector<TaskThread*> m_threads;

	// Currently executing task pool
	TaskPool* m_taskPool;

	// Whether we are currently executing or not
	bool m_executing;

	// Internal mutex for task pool access
	std::mutex m_mutex;
};

#endif