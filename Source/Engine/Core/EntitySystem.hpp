
#ifndef entitysystem_hpp
#define entitysystem_hpp

/**
 * The entity system keeps track of all entities and assigns unique entity IDs
 */
class GIGA_API EntitySystem : public System {
public:
    EntitySystem() : m_lastEntityID(0) { }
    ~EntitySystem() = default;
    
    GIGA_CLASS_NAME("EntitySystem")
    
    /**
     * Create a new entity with a given name
     */
    Entity* CreateEntity(std::string name);
    
    /**
     * Add an existing entity to the system
     */
    void AddEntity(Entity* entity);
    
    /**
     * Remove an entity from the system (does not delete it)
     */
    void RemoveEntity(Entity* entity);
    
    /**
     * Clear the entity system entirely (remove all objects)
     */
    void Clear();
    
    /**
     * Find an entity by ID or name
     */
    Entity* FindEntity(int id);
    Entity* FindEntity(std::string name);
    
    /**
     * Scripting integration
     */
    static Variant* FindEntity(Variant* object, int argc, Variant** argv);
    
protected:
    // Internal pool of entities
    ObjectPool<Entity> m_entities;
    
    // Current (last assigned) entity ID
    int m_lastEntityID;
};

#endif
