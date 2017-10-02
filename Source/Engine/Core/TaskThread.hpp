
#ifndef taskthread_hpp
#define taskthread_hpp

class ThreadPool;

class GIGA_API TaskThread {
public:
    TaskThread();
    ~TaskThread() = default;
    
    /**
     * Execute thread function
     */
    void Start(int threadID);

	/**
	 * Thread initialization
	 */
	virtual void Initialize() { }

	/**
	 * Thread shutdown
	 */
	virtual void Shutdown() { }

	/**
	 * Thread should terminate
	 */
	bool IsTerminated() { return m_terminated; }
	void Terminate() { m_terminated = true; }
    
    friend class ThreadPool;
    
protected:
    // Internal run function
    static void Run(TaskThread* thread);
    
protected:
    // Internal thread
    std::thread m_thread;
    
    // The thread pool we are a part of
    ThreadPool* m_threadPool;

	// Thread ID
	int threadID;

	// Thread should terminate
	bool m_terminated;
};

#endif
