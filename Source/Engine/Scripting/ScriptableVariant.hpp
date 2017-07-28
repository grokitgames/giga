
#ifndef scriptablevariant_hpp
#define scriptablevariant_hpp

/**
 * Extension of our base variant class to integrate with V8's types
 */
class ScriptableVariant : public Variant {
public:
    // JS types
    enum JSType {
        VAR_INT = Variant::Type::VAR_LASTTYPE,
        VAR_FUNCTION
    };
    
    /**
     * Constructor
     */
    ScriptableVariant(v8::Local<v8::Value> value);
    
    /**
     * Set operators
     */
    Variant& operator =(v8::Local<v8::Value> rhs);
    
    /**
     * Get value in V8 friendly format
     */
    v8::Local<v8::Value> GetValue();
    
    /**
     * Checker
     */
    bool IsFunction() { return(m_type == VAR_FUNCTION); }
    
};

#endif
