
#ifndef component_hpp
#define component_hpp

class Entity;
class Component;

/**
 * Internal typedefs for creating and removing components
 */
typedef Component* (*ComponentCreateFunc)(void);
typedef void(*ComponentRemoveFunc)(Component* component);

/**
 * Define a component type
 */
struct GIGA_API ComponentType {
    std::string name;
    ComponentCreateFunc createFunc;
    ComponentRemoveFunc removeFunc;
};

/**
 * Base component class that can be added to entity objects
 */
class GIGA_API Component : public ScriptableObject {
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
     * Get status of whether this component has been updated
     */
    bool GetUpdatedFlag() { return m_updated; }
    
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
    
public:
    /**
     * Register a global component type that can be added to entities - create/remove likely in system
     */
    static void RegisterComponentType(std::string type, ComponentCreateFunc f1, ComponentRemoveFunc f2);
    
    /**
     * Create a new component by string type
     */
    static Component* CreateComponent(std::string type);
    
protected:
    /**
     * Mark this component as updated
     */
    void MarkUpdated(bool updated) { m_updated = updated; }
    
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
