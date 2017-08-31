
#ifndef scriptcomponent_hpp
#define scriptcomponent_hpp

/**
 * Script component to be added to entities
 */
class GIGA_API ScriptComponent : public Component {
public:
    ScriptComponent();
    ~ScriptComponent();
    
    GIGA_CLASS_NAME("ScriptComponent");
    
    /**
     * A cached/linked function from the script
     */
    struct ScriptFunction {
        std::string name;
        v8::Persistent<v8::Function, v8::CopyablePersistentTraits<v8::Function>> func;
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
     * Clone into another component
     */
    ScriptComponent* Clone();
    
    /**
     * Call a function inside this component
     */
    void CallFunction(std::string function, int argc, Variant** argv);
    
    /**
     * Set a global variable inside our context
     */
    void SetGlobal(std::string name, Variant* value);
    
protected:
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

	// Whether this script has been initialized yet
	bool m_initialized;
};

#endif
