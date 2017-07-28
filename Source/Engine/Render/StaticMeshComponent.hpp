
#ifndef staticmeshcomponent_hpp
#define staticmeshcomponent_hpp

/**
 * Static mesh component to be added to entities
 */
class StaticMeshComponent : public SceneComponent {
public:
    StaticMeshComponent();
    ~StaticMeshComponent() = default;
    
    GIGA_CLASS_NAME("StaticMeshComponent");
    
    /**
     * Instantiate an instance from a mesh
     */
    virtual void Instantiate(Mesh* mesh);
    
    /**
     * Set data bindings
     */
    virtual void UpdateObjectDataMappings();
    
    /**
     * Clone this mesh component into another
     */
    virtual StaticMeshComponent* Clone();
    
    /**
     * Scripting integration
     */
    static Variant* Instantiate(Variant* obj, int argc, Variant** argv);
    
protected:
    /**
     * Update bounding box
     */
    virtual void UpdateBoundingBox();
    
protected:
    // Child meshes
    std::vector<StaticMeshComponent*> m_children;
    
    // The mesh object our component is based on
    Mesh* m_mesh;
    
    // Oriented bounding box
    BoundingBox m_obb;
};

#endif
