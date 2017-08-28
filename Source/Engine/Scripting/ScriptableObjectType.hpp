
#ifndef scriptableobjecttype_hpp
#define scriptableobjecttype_hpp

class ScriptableObject;

/**
 * A "getter" function template
 */
typedef Variant* (*ScriptObjectGetterFunc)(std::string prop, Variant* obj);

/**
 * A "setter" function template
 */
typedef void (*ScriptObjectSetterFunc)(std::string prop, Variant* obj, Variant* val);

/**
 * A generic callback function template
 */
typedef Variant* (*ScriptObjectFunc)(Variant* obj, int argc, Variant** argv);

/**
 * A defined interface to the scripting system
 */
class GIGA_API ScriptableObjectType {
public:
    ScriptableObjectType(std::string name);
    ~ScriptableObjectType() = default;
    
    /**
     * Storage for getter and setter to variable pairs
     */
    class GIGA_API ScriptObjectCallbackPair {
    public:
        ScriptObjectCallbackPair() : getter(0), setter(0) { }
        ~ScriptObjectCallbackPair() = default;
        
    public:
        std::string variableName;
        ScriptObjectGetterFunc getter;
        ScriptObjectSetterFunc setter;
    };
    
    /**
     * Storage for callback functions
     */
    class GIGA_API ScriptFunctionCallback {
    public:
        ScriptFunctionCallback() : func(0) { }
        ~ScriptFunctionCallback() = default;
        
    public:
        std::string variableName;
        ScriptObjectFunc func;
    };
    
    /**
     * Get/set the name of this interface as used in JS
     */
    std::string GetName() { return m_typeName; }
    void SetName(std::string name) { m_typeName = name; }
    
    /**
     * Start a new template
     */
    void StartTemplate();
    
    /**
     * End our template
     */
    void EndTemplate();
    
    /**
     * Called from JS to create a new version of our object to pass back
     */
    static void New(const v8::FunctionCallbackInfo<v8::Value>& info);
    
    /**
     * Identify whether this template is for a static global that won't have a "this" pointer
     */
    void SetStatic(bool global) { m_static = global; }

	/**
	 * Set an object as transient (delete after JS is done with it)
	 */
	void SetTransient(bool transient) { m_transient = transient; }
    
    /**
     * Add this template to a new context
     */
    void AddToContext(v8::Local<v8::Context> context);
    
    /**
     * Add an object variable (ie. this.var) to our object with callbacks for get/set
     */
    void AddObjectVariable(std::string name, ScriptObjectGetterFunc getter, ScriptObjectSetterFunc setter);
    
    /**
     * Add an object function to our template (ie. this.DoSomething())
     */
    void AddObjectFunction(std::string name, ScriptObjectFunc func);
    
    /**
     * Add a static variable to our type name (ie. Time.deltaTime) with a getter only
     */
    void AddStaticVariable(std::string name, ScriptObjectGetterFunc getter);
    
    /**
     * Add a static function to our type (ie. Time.GetTime())
     */
    void AddStaticFunction(std::string name, ScriptObjectFunc func);
    
    /**
     * Handle static function callbacks (ie. Time.GetTime())
     */
    static void HandleStaticFunctionCallback(const v8::FunctionCallbackInfo<v8::Value>& info);
    
    /**
     * Handle object function callbacks (ie. this.DoSomething())
     */
    static void HandleObjectFunctionCallback(const v8::FunctionCallbackInfo<v8::Value>& info);
    
    /**
     * Handle object getters (call internal callbacks based on name)
     */
    static void HandleObjectGetter(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value>& info);
    
    /**
     * Handle object setters (call internal callbacks based on name)
     */
    static void HandleObjectSetter(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info);
    
    /**
     * Handle static getter calls (call internal callback based on name)
     */
    static void HandleStaticGetter(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value>& info);
    
    /**
     * Get our function list
     */
    std::vector<ScriptFunctionCallback*>& GetFunctionList() { return m_funcList; }
    
    /**
     * Get our variable list
     */
    std::vector<ScriptObjectCallbackPair*>& GetVariableList() { return m_varList; }
    
    // Set the constructor for this type back to a C++ object
    template<class T> void SetConstructor() {
        if(m_static)
            m_createFunction = 0;
        else
            m_createFunction = &CreateObject<T>;
    }
    
    /**
     * Create a JavaScript object to pass back to V8
     */
    v8::Local<v8::Value> CreateJSObject();
    
protected:
    // Internal function to call a C++ constructor
    typedef ScriptableObject* (*ScriptObjectCreateFunc)();
    
    // Create a new C++ object based on a registered script interface type
    template<typename T> static ScriptableObject* CreateObject() { return new T; }
    
protected:
    // The name of this type
    std::string m_typeName;
    
    // Link back to our internal CreateObject function
    ScriptObjectCreateFunc m_createFunction;
    
    // The function template
    v8::Persistent<v8::FunctionTemplate, v8::CopyablePersistentTraits<v8::FunctionTemplate>> m_functionTemplate;
    
    // Our constructor to create new objects of this type
    v8::Persistent<v8::Function, v8::CopyablePersistentTraits<v8::Function>> m_constructor;
    
    // Linked list of getter/setter combinations for vars
    std::vector<ScriptObjectCallbackPair*> m_varList;
    
    // Linked list of function callbacks
    std::vector<ScriptFunctionCallback*> m_funcList;
    
    // Define whether this class is a static global only (New should never be called)
    bool m_static;

	// Whether the type is transient (should be garbage collected after V8 is done with it)
	bool m_transient;
};

#endif
