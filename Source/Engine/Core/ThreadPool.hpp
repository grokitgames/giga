
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
    
    TaskPool* GetTaskPool();
    
    void SetExecuting(bool executing);

protected:
	// Threads
	std::vector<TaskThread*> m_threads;
    
    // The task pool we are executing
    TaskPool* m_taskPool;

	// Whether we are currently executing or not
	bool m_executing;
    std::mutex m_mutex;
};

#endif
