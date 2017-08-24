
#ifndef staticmeshcomponent_hpp
#define staticmeshcomponent_hpp

/**
 * Static mesh component to be added to entities
 */
class StaticMeshComponent : public RenderComponent {
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
    virtual void SetDataMappings();

	/**
	 * Update once data is bound
	 */
	void UpdateFromDataMappings();
    
    /**
     * Clone this mesh component into another
     */
    virtual StaticMeshComponent* Clone();
    
    /**
     * Enable/disable lighting on this mesh
     */
    void SetLighting(bool lighting) { m_applyLighting = lighting; }
    bool HasLighting() { return m_applyLighting; }
    
    /**
     * Get bounding box
     */
    BoundingBox& GetBoundingBox() { return m_obb; }
    
    /**
     * Get child mesh list
     */
    std::vector<StaticMeshComponent*>& GetChildren() { return m_children; }
    
    /**
     * The mesh this class is based on
     */
    Mesh* GetMesh() { return m_mesh; }
    
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

	// Mesh instantiated
	bool m_initialized;
    
    // Oriented bounding box
    BoundingBox m_obb;
    
    // Whether to apply lighting to this component or not
    bool m_applyLighting;
};

#endif
