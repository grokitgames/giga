
#ifndef componenttemplate_hpp
#define componenttemplate_hpp

/**
 * A component template, which is simply a component create from a ComponentTemplateType
 */
class ComponentTemplate : public Component {
public:
    ComponentTemplate() = default;
    ~ComponentTemplate() = default;
    
    virtual std::string GetGigaName();
	virtual std::string GetStorableTypeName();

	/**
	 * Initialize a component by string name
	 */
	void InitializeComponent(std::string name);
    
    /**
     * Initialize when created from scripting system
     */
    void InitializeScriptableObject(std::string name);
    
    /**
     * Initialize when created from data loader system
     */
    void InitializeStorableObject(std::string name);
    
    /**
     * Setup from type
     */
    void SetupFromType(std::string name);

	/**
	 * Copy
	 */
	void Copy(Component* component);
    
    /**
     * Register a new component template type
     */
    static void RegisterComponentTemplateType(std::string name, ComponentTemplateType* type);
    
    /**
     * Generic get/set function callbacks
     */
    static Variant* Getter(std::string var, Variant* obj);
    static void Setter(std::string var, Variant* obj, Variant* val);
    
    friend class ComponentTemplateType;
    
protected:
    /**
     * All registered component template types
     */
    static std::map<std::string, ComponentTemplateType*> m_types;
    
protected:
    // The template type of this particular component
    ComponentTemplateType* m_type;
    
    // Type (for lookup)
    std::map<std::string, int> m_fieldTypes;
    
    // Values
    std::map<std::string, std::string> m_objectStringFields;
    std::map<std::string, int> m_objectIntFields;
    std::map<std::string, float> m_objectFloatFields;
    std::map<std::string, vector2> m_objectVector2Fields;
    std::map<std::string, vector3> m_objectVector3Fields;
    std::map<std::string, quaternion> m_objectQuaternionFields;
};

#endif 
