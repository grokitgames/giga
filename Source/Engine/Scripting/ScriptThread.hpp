
#ifndef scriptthread_hpp
#define scriptthread_hpp

class ScriptThread : public TaskThread {
public:
	ScriptThread();
	~ScriptThread();

	/**
	* Thread initialization
	*/
	void Initialize();

	/**
	* Thread shutdown
	*/
	void Shutdown();
    
    /**
     * Get isolate
     */
    v8::Isolate* GetIsolate() { return m_isolate; }
    
    /**
     * Lock thread
     */
    void Lock();
    
    /**
     * Check lock
     */
    bool IsLocked();
    
    /**
     * Unlock
     */
    void Unlock();
    
    /**
     * Get an implementation by name
     */
    ScriptableObjectImpl* GetScriptableImpl(std::string name);
    
    /**
     * Find a scriptable object
     */
    v8::Local<v8::Object> GetCachedObject(ScriptableObject* obj);

protected:
	// V8 isolated execution environment (own heap, stack, GC, etc.)
	v8::Isolate* m_isolate;
    v8::Locker* m_locker;

	// Scriptable object types initialized in this isolate
	std::vector<ScriptableObjectImpl*> m_impls;
    
    // Objects inside of this thread's context
    std::map<ScriptableObject*, v8::Persistent<v8::Object, v8::CopyablePersistentTraits<v8::Object>>> m_vars;
};

#endif
