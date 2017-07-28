
#ifndef transformcomponent_hpp
#define transformcomponent_hpp

/**
 * Our basic transformation component, applied to all things living in 3D space
 */
class GIGA_API TransformComponent : public Component {
public:
    TransformComponent() = default;
    ~TransformComponent() = default;
    
    GIGA_CLASS_NAME("TransformComponent");
    
    /**
     * Update the data field mappings to bind to member variables
     */
    void UpdateObjectDataMappings();
    
    /**
     * Get our transform
     */
    Transform* GetTransform() { return &m_transform; }
    void SetTransform(Transform* transform);
    
    /**
     * Clone this component into another copy
     */
    TransformComponent* Clone();
    
    /**
     * Scripting integration
     */
    static Variant* GetTransform(Variant* obj);
    static void SetTransform(Variant* obj, Variant* val);
    
protected:
    Transform m_transform;
};

#endif
