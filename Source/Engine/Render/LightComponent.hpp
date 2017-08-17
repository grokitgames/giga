
#ifndef light_hpp
#define light_hpp

class Scene;

/**
 * Defines a light in our world/scene
 */
class GIGA_API LightComponent : public RenderComponent {
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
     * "Camera" for matrices and frustums
     */
    CameraComponent* m_camera;
    
    /**
     * Shadow map texture
     */
    Texture* m_shadowMap;
    
    /**
     * Depth texture
     */
    Texture2D* m_depthTexture;
    
    /**
     * Framebuffer for doing depth pass
     */
    Framebuffer* m_framebuffer;
    
    /**
     * Render pass for shadow passes
     */
    ShadowPass* m_shadowPass;
    
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
