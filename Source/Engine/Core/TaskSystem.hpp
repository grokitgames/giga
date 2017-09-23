
#ifndef tasksystem_hpp
#define tasksystem_hpp

class GIGA_API TaskSystem : public System {
public:
	TaskSystem();
    ~TaskSystem() = default;
    
    GIGA_CLASS_NAME("TaskSystem");
    
    /**
     * Initialize with a number of threads
     */
    void Initialize(int threads);
    
    /**
     * Execute a task pool to completion
     */
    void Execute(TaskPool* pool);
    
    /**
     * Get next available task
     */
    Task* GetNextTask();
    
protected:
    // Threads
    std::vector<TaskThread*> m_threads;
    
    // Current task pool
    TaskPool* m_taskPool;
    
    // Whether we are currently executing or not
    bool m_executing;
    
    // Internal mutex for task pool access
    std::mutex m_mutex;
};

#endif
