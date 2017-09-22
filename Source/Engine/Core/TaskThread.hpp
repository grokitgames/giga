
#ifndef taskthread_hpp
#define taskthread_hpp

class TaskThread {
public:
    TaskThread() = default;
    ~TaskThread() = default;
    
    /**
     * Execute thread function
     */
    void Start();
    
protected:
    // Internal run function
    static void Run();
    
protected:
    // Internal thread
    std::thread m_thread;
};

#endif
