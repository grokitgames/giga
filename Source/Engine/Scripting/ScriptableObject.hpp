
#ifndef scriptableobject_hpp
#define scriptableobject_hpp

class ScriptThread;

/**
 * Scriptable object base class - all C++ classes used in JS should inherit this
 */
class GIGA_API ScriptableObject : public GigaObject {
public:
    virtual ~ScriptableObject() = default;
    
    /**
     * Initialize a newly created object
     */
    virtual void InitializeScriptableObject(std::string name) { }
    
    /**
     * An overridable function for objects created with "new" and parameters passed
     */
    virtual void Initialize(Variant** argv, int argc) { }
    
    /**
     * Get the interface to this type
     */
    ScriptableObjectType* GetScriptType() { return m_scriptType; }
    
    /**
     * Return this object as a Javascript object
     */
    v8::Local<v8::Object> GetJSObject();

	/**
	 * Whether an object can be deleted
	 */
	bool IsDeleted() { return m_deleted; }
    
public:
    /**
     * Unwrap a v8::object back into a C++ object
     */
    static inline ScriptableObject* Unwrap(v8::Local<v8::Object> handle) {
        assert(!handle.IsEmpty());
        assert(handle->InternalFieldCount() > 0);
        
        // Cast to ObjectWrap before casting to T.  A direct cast from void
        // to T won't work right when T has more than one base class.
        void* ptr = handle->GetAlignedPointerFromInternalField(0);
        ScriptableObject* wrap = static_cast<ScriptableObject*>(ptr);
        return(wrap);
    }
    
    /**
     * Wrap a c++ object to pass back into v8
     */
    inline void Wrap(v8::Local<v8::Value> handle) {
        v8::Local<v8::Object> inthandle = handle.As<v8::Object>();
        assert(inthandle->InternalFieldCount() > 0);
        
        inthandle->SetAlignedPointerInInternalField(0, this);
    }

	/**
	 * Callback for garbage collected objects from V8
	 */
	static void __GCCallback(const v8::WeakCallbackInfo<ScriptableObject>& data);
    
protected:
    // Protected constructor so we can't create these directly
    ScriptableObject();
    
protected:
    // Script type interface
    ScriptableObjectType* m_scriptType;

	// Is deleted
	bool m_deleted;

	// Cached JS variables for various threads
	std::map<ScriptThread*, v8::Persistent<v8::Object, v8::CopyablePersistentTraits<v8::Object>>> m_cached;
};

#endif 
