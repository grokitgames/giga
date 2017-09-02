
#ifndef scene_hpp
#define scene_hpp

class RenderSystem;

class GIGA_API Scene : public ScriptableObject {
public:
    Scene() = default;
    ~Scene() = default;

	GIGA_CLASS_NAME("Scene");
    
    /**
     * Getters
     */
    std::list<StaticMeshComponent*>& GetMeshes() { return m_meshes.GetList(); }
    std::list<LightComponent*>& GetLights() { return m_lights.GetList(); }
    // std::vector<BillboardComponent*>& GetBillboards();
    
    /**
     * Get/set ambient lighting
     */
    void SetAmbientLight(vector3 color) { m_ambient = color; }
    vector3 GetAmbientLight() { return m_ambient; }
    
    /**
     * Get/set camera
     */
    void SetActiveCamera(CameraComponent* camera) { m_activeCamera = camera; }
    CameraComponent* GetActiveCamera() { return m_activeCamera; }
    
    friend class RenderSystem;
    
protected:
    // Mesh components
    ObjectPool<StaticMeshComponent> m_meshes;
    
    // Camera components
    ObjectPool<CameraComponent> m_cameras;
    
    // Lights
    ObjectPool<LightComponent> m_lights;
    
    // Current ambient lighting
    vector3 m_ambient;
    
    // Current camera
    CameraComponent* m_activeCamera;
};

#endif 
