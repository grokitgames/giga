
#ifndef opengldeferredlightingpass_hpp
#define opengldeferredlightingpass_hpp

/**
 * Built-in deferred lighting pass (one pass per light)
 */
class GIGA_API OpenGLDeferredLightingPass : public PostProcessPass {
public:
    OpenGLDeferredLightingPass() = default;
    ~OpenGLDeferredLightingPass() = default;
    
    /**
     * Initialize
     */
    void InitializeShader();
    
    /**
     * Set textures
     */
    void SetDiffuseTexture(Texture2D* diffuse) { m_diffuse = diffuse; }
    void SetNormalTexture(Texture2D* normal) { m_normal = normal; }
    void SetPositionTexture(Texture2D* position) { m_position = position; }
    
    /**
     * Get textures
     */
    Texture2D* GetLightingTexture();
    
    /**
     * Render
     */
    void Render(Scene* scene);
    
protected:
    // Pointers to textures from main render
    Texture2D* m_diffuse;
    Texture2D* m_normal;
    Texture2D* m_position;
};

#endif
