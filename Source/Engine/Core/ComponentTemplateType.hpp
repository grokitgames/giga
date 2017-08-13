
#ifndef componenttype_hpp
#define componenttype_hpp

class ComponentTemplate;

/**
 * A generic component class to be used for creating new component types from JS
 */
class ComponentTemplateType : public ScriptableObject {
public:
    ComponentTemplateType();
    ~ComponentTemplateType() = default;
    
    GIGA_CLASS_NAME("ComponentTemplateType");
    
    /**
     * Set the name (data loader and JS)
     */
    void SetName(std::string name);
    std::string GetName() { return m_name; }
    
    /**
     * Create a new data field
     */
    void CreateField(std::string name, int type);
    
    /**
     * Register our new component template type with the data loader and scripting system
     */
    void Register(int typeID);
    
    /**
     * Initialize a component
     */
    void InitializeComponent(ComponentTemplate* ct);
    
    /**
     * Scripting integration
     */
    static Variant* CreateField(Variant* obj, int argc, Variant** argv);
    static Variant* SetName(Variant* obj, int argc, Variant** argv);
    static Variant* Register(Variant* obj, int argc, Variant** argv);
    
protected:
    // Name of our class
    std::string m_name;
    
    // Type (for lookup)
    std::map<std::string, int> m_objectVarTypes;
    
    // Values
    std::vector<std::string> m_objectStringFields;
    std::vector<std::string> m_objectIntFields;
    std::vector<std::string> m_objectFloatFields;
    std::vector<std::string> m_objectVector2Fields;
    std::vector<std::string> m_objectVector3Fields;
    std::vector<std::string> m_objectQuaternionFields;
    
    // References to our storable and scriptable object type definitions
    StorableObjectType* m_storableType;
    ScriptableObjectType* m_scriptableType;
};

#endif
