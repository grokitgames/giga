
#ifndef entity_hpp
#define entity_hpp

/**
 * Base entity class that holds component objects
 */
class GIGA_API Entity : public ScriptableObject {
public:
    Entity();
    ~Entity();
    
    GIGA_CLASS_NAME("Entity");
    
    /**
     * Get/set entity name
     */
    void SetName(std::string name) { m_name = name; }
    std::string GetName() { return m_name; }
    
    /**
     * Get/set numeric ID (generally done by entity system)
     */
    void SetID(int id) { m_id = id; }
    int GetID() { return m_id; }
    
    /**
     * Get all components attached to this entity
     */
    std::vector<Component*>& GetComponents() { return m_components; }
    
    /**
     * Add a new (existing) component
     */
    void AddComponent(Component* component);
    
    /**
     * Remove an existing component
     */
    void RemoveComponent(Component* component);
    
    /**
     * Find a component by type
     */
    template<class T> T* FindComponent() {
        for (int i = 0; i < m_components.size(); i++) {
            T* object = dynamic_cast<T*>(m_components[i]);
            if (object) {
                return(object);
            }
        }
        
        return(0);
    }
    
    /**
     * Scripting integration
     */
    static Variant* FindComponent(Variant* object, int argc, Variant** argv);
    
private:
    // Our list of components attached to this entity
    std::vector<Component*> m_components;
    
    // Internal entity ID
    int m_id;
    
    // Entity name
    std::string m_name;
};

#endif
