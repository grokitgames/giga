
#ifndef scriptcomponent_hpp
#define scriptcomponent_hpp

class ScriptThread;

/**
 * Script component to be added to entities
 */
class GIGA_API ScriptComponent : public Component {
public:
    ScriptComponent();
    ~ScriptComponent();
    
    GIGA_CLASS_NAME("ScriptComponent");

	// The storage of an event handler
	struct ScriptEventHandler {
		std::string type;
		int entityID;
		std::string func;
	};
    
    /**
     * Initialize a ScriptComponent from a Script
     */
    void Initialize(Script* script);
    
    /**
     * Add a newly defined inteface
     */
    void AddToContext(ScriptableObjectType* type);
    
    /**
     * Copy into another component
     */
    void Copy(Component* component);
    
    /**
     * Call a function inside this component
     */
    void CallFunction(std::string function, int argc, Variant** argv);
    
    /**
     * Set a global variable inside our context
     */
    void SetGlobal(std::string name, Variant* value);

	/**
	* Set data bindings
	*/
	void SetDataMappings();

	/**
	 * Add event handler
	 */
	void RegisterEventHandler(std::string type, std::string func, int entityID = 0);

	/**
	 * Handle event
	 */
	void ProcessEvent(Event* ev);
    
    struct ScriptFunction {
        std::string funcName;
        v8::Persistent<v8::Function> func;
    };
    
    /**
     * Task system integration
     */
    static bool Update(TaskThread* thread, Variant* obj, int argc, Variant** argv);
    
protected:
	// The thread this component was created on
	ScriptThread* m_thread;

    // Our script
    Script* m_scriptSource;
    
    // Our internal script object
    v8::Persistent<v8::Script, v8::CopyablePersistentTraits<v8::Script>> m_script;
    
    // Our internal context
    v8::Persistent<v8::Context, v8::CopyablePersistentTraits<v8::Context>> m_context;
    
    // List of functions from inside of the script
    std::vector<ScriptFunction*> m_functions;
    
    // List of accessible (undefined) global variables
    std::vector<std::string> m_globals;

	// Registered event handlers from scripts
	std::vector<ScriptEventHandler*> m_eventHandlers;

	// Whether this script has been initialized yet
	bool m_initialized;
};

#endif
