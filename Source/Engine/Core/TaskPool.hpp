
#ifndef taskpool_hpp
#define taskpool_hpp

class GIGA_API TaskPool {
public:
    TaskPool() = default;
    ~TaskPool();
    
    /**
     * Add a task to the pool
     */
    void Push(Task* task);
    
    /**
     * Whether there are tasks to execute
     */
    bool HasTasks();
    
    /**
     * Remove a task from the pool
     */
    Task* Pop();
    
protected:
    std::vector<Task*> m_tasks;
};

#endif
