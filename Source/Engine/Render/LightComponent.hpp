
#ifndef light_hpp
#define light_hpp

class Scene;

/**
 * Defines a light in our world/scene
 */
class GIGA_API LightComponent {
public:
    virtual ~LightComponent() = default;
    
    /**
     * Light types
     */
    enum {
        LIGHT_DIRECTIONAL = 1,
        LIGHT_POINT,
        LIGHT_SPOT
    };
    
    /**
     * Initialize
     */
    virtual void Initialize() = 0;
    
    /**
     * Create depth textures
     */
    virtual void CreateDepthTextures(Scene* scene) = 0;
    
    /**
     * Get debug mesh
     */
    StaticMeshComponent* GetDebugMesh() { return m_debugLight; }
    
    /**
     * Get/set color
     */
    void SetColor(vector3 color) { m_color = color; }
    vector3& GetColor() { return m_color; }
    
    /**
     * Get/set distance
     */
    void SetAttenuation(float distance) { m_attenuation = distance; }
    float GetAttenuation() { return m_attenuation; }
    
protected:
    // Can't create directly
    LightComponent();
    
    /**
     * Recalculate internal matrices
     */
    virtual void RecalculateMatrices() = 0;
    
protected:
    /**
     * The color of light
     */
    vector3 m_color;
    
    /**
     * Light type
     */
    unsigned int m_type;
    
    /**
     * Attenuation (distance to 0% light)
     */
    float m_attenuation;
    
    /**
     * Number of passes (textures, matrices, etc.)
     */
    unsigned int m_passes;
    
    /**
     * Light space matrix
     */
    std::vector<matrix4> m_lightSpaceMatrices;
    
    /**
     * Bounding frustum for this light
     */
    std::vector<Frustum> m_lightFrustums;
    
    /**
     * Shadow map texture
     */
    std::vector<Texture*> m_depthTextures;
    
    /**
     * A renderable light volume to see where the light is
     */
    StaticMeshComponent* m_debugLight;
    
    /**
     * The mesh our instance is based on
     */
    Mesh* m_mesh;
};

#endif
