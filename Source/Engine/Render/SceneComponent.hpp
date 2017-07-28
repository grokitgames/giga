
#ifndef scenecomponent_hpp
#define scenecomponent_hpp

class GIGA_API SceneComponent : public Component {
public:
    SceneComponent();
    ~SceneComponent() = default;
    
    /**
     * Get our (local) transform object
     */
    Transform* GetTransform() { return &m_transform; }
    void SetTransform(Transform* transform);
    
    /**
     * Override entity set so we can check for a transformcomponent
     */
    void OnEntitySet();
    
    /**
     * Get/set our position in local or world space
     */
    vector3 GetWorldPosition() const;
    void SetWorldPosition(vector3 position);
    
    vector3 GetLocalPosition() const;
    void SetLocalPosition(vector3 position);
    
    /**
     * Get/set our rotation in local or world space
     */
    quaternion GetWorldRotation() const;
    void SetWorldRotation(quaternion rotation);
    
    quaternion GetLocalRotation() const;
    void SetLocalRotation(quaternion rotation);
    
    /**
     * Get/set our scaling vector in local or world space
     */
    vector3 GetWorldScaling() const;
    void SetWorldScaling(vector3 scaling);
    
    vector3 GetLocalScaling() const;
    void SetLocalScaling(vector3 scaling);
    
    /**
     * Get our model matrix (in world space)
     */
    matrix4 GetModelMatrix();
    
    /**
     * Scripting functions
     */
    static Variant* GetTransform(Variant* obj);
    static void SetTransform(Variant* obj, Variant* val);
    
protected:
    // A pointer to an object's transformcomponent (if applicable)
    TransformComponent* m_transformComponent;
    
    // The relative transform to the transformcomponent
    Transform m_transform;
    
    // Whether to apply lighting to this component or not
    bool m_lit;
};

#endif
