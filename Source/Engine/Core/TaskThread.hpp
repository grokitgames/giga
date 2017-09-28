
#ifndef taskthread_hpp
#define taskthread_hpp

class GIGA_API TaskThread {
public:
    TaskThread();
    ~TaskThread() = default;
    
    /**
     * Execute thread function
     */
    void Start();
    
protected:
    // Internal run function
    static void Run(TaskThread* thread);
    
protected:
    // Internal thread
    std::thread m_thread;

	// Thread ID
	int threadID;
};

#endif
