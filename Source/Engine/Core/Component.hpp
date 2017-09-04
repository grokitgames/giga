
#ifndef component_hpp
#define component_hpp

class Entity;
class Component;

/**
 * Internal typedefs for creating and removing components
 */
typedef void (*ComponentAddFunc)(Component* component);
typedef void (*ComponentRemoveFunc)(Component* component);
typedef Component* (*ComponentCreateFunc)();

/**
 * Define a component type
 */
struct GIGA_API ComponentType {
    std::string name;
    int typeID;
	ComponentAddFunc addFunc;
	ComponentCreateFunc createFunc;
    ComponentRemoveFunc removeFunc;
};

/**
 * Base component class that can be added to entity objects
 */
class GIGA_API Component : public ScriptableObject, public StorableObject {
public:
    virtual ~Component();

	/**
	 * Initialize a new component
	 */
	virtual void InitializeComponent(std::string name) { }
    
    /**
     * Set/get parent entity
     */
    void SetParent(Entity* entity) { m_parent = entity; }
    Entity* GetParent() { return m_parent; }
    
    /**
     * Clone this component into another
     */
    Component* Clone();

	/**
	 * Copy the properties of another component into this one
	 */
	virtual void Copy(Component* component) = 0;
    
    /**
     * Interpolate from another component
     */
    virtual void Interpolate(Component* component, float amount) { }
    
    /**
     * Check whether this component is active or not
     */
    bool IsActive() { return m_active; }
	void SetActive(bool active) { m_active = active; }
    
    /**
     * Set network update state
     */
    void MarkUpdated(bool updated);
    bool HasUpdates() { return m_updated; }

	/**
	 * Add this component to a registered system
	 */
	void AddToSystem();
    
    /**
     * Overridable callback functions
     */
    
    // Called when the parent entity of this object is set or removed
    virtual void OnEntityUpdate() { }
    
    // Called when the active status of this component is updated
    virtual void OnActiveUpdate() { }
    
    // Called when other components in the same entity are added
    virtual void OnComponentAdded() { }
    
    // Called when other components in the same entity are removed
    virtual void OnComponentRemoved() { }
    
    /**
     * Register a global component type that can be added to entities - create/remove likely in system
     */
	template<class T>
    static void RegisterComponentType(std::string type, int typeID, ComponentAddFunc f1, ComponentRemoveFunc f2) {
		std::map<std::string, ComponentType*>::iterator i = m_componentTypes.begin();
		for (i; i != m_componentTypes.end(); i++) {
			if (i->second->name == type || i->second->typeID == typeID) {
				GIGA_ASSERT(false, "Component name or type ID already registered.");
			}
		}

		ComponentType* ct = new ComponentType();
		ct->name = type;
		ct->typeID = typeID;
		ct->addFunc = f1;
		ct->createFunc = &InternalCreateComponent<T>;
		ct->removeFunc = f2;

		m_componentTypes[type] = ct;
	}
    
    /**
     * Create a new component by string type
     */
    static Component* CreateComponent(std::string type);
    
    /**
     * Create a new component by type ID
     */
    static Component* CreateComponent(int typeID);
    
	/**
	 * Get type ID
	 */
	int GetTypeID();

protected:
    // Type ID
    int m_typeID;

protected:
	// Create a new component from a class type
	template<typename T> static Component* InternalCreateComponent() { return new T; }
    
protected:
    // Parent entity
    Entity* m_parent;
    
    // Whether this component is complete
    bool m_active;
    
    // Whether this component has been updated (for networking mostly)
    bool m_updated;

	// Add to system
	ComponentAddFunc m_addFunction;
    
    // Removal function
    ComponentRemoveFunc m_removeFunction;
    
private:
    // Globally registered component types (and their creation functions)
    static std::map<std::string, ComponentType*> m_componentTypes;
    
protected:
    // Private constructor to prevent direct creation
    Component();
};

#endif
