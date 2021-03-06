
#ifndef task_hpp
#define task_hpp

class TaskThread;

/**
 * Generic task callback function
 */
typedef bool(*TaskCallbackFn)(TaskThread* thread, Variant* obj, int argc, Variant** argv);

/**
 * A single task to be added to a task pool and picked up by a series of threads
 */
class GIGA_API Task {
public:
    Task();
    ~Task();
    
    /**
     * Create a new task to be executed
     */
    void Create(GigaObject* obj, TaskCallbackFn func);
    
    /**
     * Add argument
     */
    void AddArgument(Variant* arg);
    
    /**
     * Execute
     */
    bool Execute(TaskThread* thread);
    
protected:
    TaskCallbackFn m_func;
	Variant* m_obj;
    std::vector<Variant*> m_args;
};

#endif
