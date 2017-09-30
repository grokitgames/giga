
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

protected:
	// V8 isolated execution environment (own heap, stack, GC, etc.)
	v8::Isolate* m_isolate;

	// Scriptable object types initialized in this isolate
	std::vector<ScriptableObjectImpl*> m_types;
};

#endif