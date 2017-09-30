
#ifndef taskthread_hpp
#define taskthread_hpp

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
    
protected:
    // Internal run function
    static void Run(TaskThread* thread);
    
protected:
    // Internal thread
    std::thread m_thread;

	// Thread ID
	int threadID;

	// Thread should terminate
	bool m_terminated;
};

#endif
