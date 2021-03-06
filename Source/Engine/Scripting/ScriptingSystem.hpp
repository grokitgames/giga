
#ifndef scriptingsystem_hpp
#define scriptingsystem_hpp

/**
 * Scripting system, tracking interfaces to C++ and executing loaded JavaScript
 */
class GIGA_API ScriptingSystem : public System, public ScriptThread {
public:
    ScriptingSystem();
    ~ScriptingSystem();
    
    GIGA_CLASS_NAME("ScriptingSystem");
    
    /**
     * Initialize scripting system
     */
    void Initialize();
    
    /**
     * Update loop
     */
    void Update(float delta);
    
    /**
     * Find an interface definition by its class name
     */
    ScriptableObjectType* GetScriptableObjectType(std::string name);
    
    /**
     * Register an object interface to be used in the scripting environment
     */
    template<class T> void RegisterScriptableObjectType(ScriptableObjectType* type) {
        for (size_t i = 0; i < m_types.size(); i++) {
            if (m_types[i] == type) {
                return;
            }
        }
        
        type->SetConstructor<T>();
        type->EndTemplate();
        
        m_types.push_back(type);
        
        // Add to any existing script components
        std::vector<ScriptComponent*> scripts = m_scripts.GetList();
		std::vector<ScriptComponent*>::iterator i = scripts.begin();
		for (i; i != scripts.end(); i++) {
            (*i)->AddToContext(type);
        }
        
        ScriptableObjectImpl* impl = new ScriptableObjectImpl();
        impl->Create(type, this);
        
        m_impls.push_back(impl);
    }
    
    /**
     * Register a new global variable to be used in all scripts
     */
    void RegisterGlobal(std::string name, Variant* value);
    
    /**
     * Get all currently registered interfaces
     */
    std::vector<ScriptableObjectType*>& GetRegisteredTypes() { return m_types; }
    std::map<std::string, ScriptableVariant*>& GetRegisteredGlobals() { return m_globals; }
    
    /**
     * Register a scripted event handler
     */
    void RegisterEventHandler(ScriptComponent* component, std::string type, std::string func, int entityID = 0);
    
    /**
     * Generic callback fo registered events from the event system
     */
    static void EventHandler(Event* event);
    
    /**
     * Create and remove scripts
     */
    static void AddScriptComponent(Component* component);
    static void RemoveScriptComponent(Component* component);

	/**
	 * Add a transient JS object (can be garbage collected)
	 */
	void AddTransient(ScriptableObject* object);
    
protected:
    // V8 stuff
    v8::Platform* m_platform;

    // Registered script interfaces to C++ objects
    std::vector<ScriptableObjectType*> m_types;
    
    // A list of global variables to be injected into all scripts upon creation
    std::map<std::string, ScriptableVariant*> m_globals;
    
    // List of running script components
    ObjectPool<ScriptComponent> m_scripts;

	// List of transient JS objects
	ObjectPool<ScriptableObject> m_transients;
    
    // Script thread
	ThreadPool* m_threadPool;
	TaskPool* m_taskPool;
};

#endif
