
#ifndef staticmeshcomponent_hpp
#define staticmeshcomponent_hpp

/**
 * Static mesh component to be added to entities
 */
class StaticMeshComponent : public RenderComponent {
public:
    StaticMeshComponent();
    ~StaticMeshComponent();
    
    GIGA_CLASS_NAME("StaticMeshComponent");

	class StaticMeshPredictionError : public ClientPredictionError {
	public:
		StaticMeshPredictionError() = default;
		~StaticMeshPredictionError() = default;

		vector3 translation;
		quaternion rotation;
	};
    
    /**
     * Instantiate an instance from a mesh
     */
    virtual void Instantiate(Mesh* mesh);

	/**
	* Interpolate from another static mesh component
	*/
	void Interpolate(Component* current, Component* next, float amount);
	ClientPredictionError* CheckPredictionError(Component* current);
	void AdjustForPredictionError(ClientPredictionError* error, float interpolation);
    
    /**
     * Set data bindings
     */
    virtual void SetDataMappings();

	/**
	 * Update once data is bound
	 */
	void UpdateFromDataMappings();
    
    /**
     * Copy this mesh component into another
     */
    void Copy(Component* component);
    
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
