
#ifndef component_hpp
#define component_hpp

class Entity;
class Component;

/**
 * Internal typedefs for creating and removing components
 */
typedef Component* (*ComponentCreateFunc)(std::string type);
typedef void(*ComponentRemoveFunc)(Component* component);

/**
 * Define a component type
 */
struct GIGA_API ComponentType {
    std::string name;
    int typeID;
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
     * Set/get parent entity
     */
    void SetParent(Entity* entity) { m_parent = entity; }
    Entity* GetParent() { return m_parent; }
    
    /**
     * Clone this component into another
     */
    virtual Component* Clone() = 0;
    
    /**
     * Check whether this component is active or not
     */
    bool IsActive() { return m_active; }
    
    /**
     * Set network update state
     */
    void MarkUpdated(bool updated);
    bool HasUpdates() { return m_updated; }
    
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
    static void RegisterComponentType(std::string type, int typeID, ComponentCreateFunc f1, ComponentRemoveFunc f2);
    
    /**
     * Create a new component by string type
     */
    static Component* CreateComponent(std::string type);
    
    /**
     * Create a new component by type ID
     */
    static Component* CreateComponent(int typeID);
    
public:
    // Type ID
    int typeID;
    
protected:
    // Parent entity
    Entity* m_parent;
    
    // Whether this component is complete
    bool m_active;
    
    // Whether this component has been updated (for networking mostly)
    bool m_updated;
    
    // Removal function
    ComponentRemoveFunc removeFunction;
    
private:
    // Globally registered component types (and their creation functions)
    static std::map<std::string, ComponentType*> m_componentTypes;
    
protected:
    // Private constructor to prevent direct creation
    Component();
};

#endif
