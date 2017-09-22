
#ifndef task_hpp
#define task_hpp

/**
 * Generic task callback function
 */
typedef void(*TaskCallbackFn)(Variant* obj, int argc, Variant** argv);

/**
 * A single task to be added to a task pool and picked up by a series of threads
 */
class Task {
public:
    Task();
    ~Task() = default;
    
    /**
     * Create a new task to be executed
     */
    void Create(Variant* obj, TaskCallbackFn func);
    
    /**
     * Add argument
     */
    void AddArgument(Variant* arg);
    
    /**
     * Execute
     */
    void Execute();
    
protected:
    TaskCallbackFn m_func;
    Variant* m_obj;
    std::vector<Variant*> m_args;
};

#endif
