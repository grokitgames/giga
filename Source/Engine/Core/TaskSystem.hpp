
#ifndef tasksystem_hpp
#define tasksystem_hpp

class GIGA_API TaskSystem : public System {
public:
	TaskSystem();
    ~TaskSystem() = default;
    
    GIGA_CLASS_NAME("TaskSystem");

	/**
	 * Initialize with a certain number of internal threads
	 */
	void Initialize(int threads);

    /**
     * Execute a task pool to completion
     */
    void Execute(TaskPool* taskPool, ThreadPool* threadPool = 0);
    
protected:
    // Current thread pool
	ThreadPool* m_threadPool;
};

#endif
