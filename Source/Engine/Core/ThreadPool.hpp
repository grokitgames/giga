
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
    
    TaskPool* LockTaskPool();
	void UnlockTaskPool();
    
	void MarkCompleted(Task* task);

protected:
	// Threads
	std::vector<TaskThread*> m_threads;
    
    // The task pool we are executing
    TaskPool* m_taskPool;

	// Number of completed tasks
	int m_poolSize;
	int m_completed;

    std::mutex m_mutex;
};

#endif
