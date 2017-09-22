
#ifndef taskpool_hpp
#define taskpool_hpp

class TaskPool {
public:
    TaskPool() = default;
    ~TaskPool() = default;
    
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
