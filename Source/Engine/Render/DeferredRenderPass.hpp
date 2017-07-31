
#ifndef deferredrenderpass_hpp
#define deferredrenderpass_hpp

/**
 * Built-in deferred rendering pass (combination of previous g-buffer pass + lighting)
 */
class GIGA_API DeferredRenderPass : public PostProcessPass {
public:
    DeferredRenderPass() = default;
    ~DeferredRenderPass() = default;
    
    /**
     * Set textures
     */
    void SetDiffuseTexture(Texture2D* diffuse) { m_diffuse = diffuse; }
    void SetNormalTexture(Texture2D* normal) { m_normal = normal; }
    void SetPositionTexture(Texture2D* position) { m_position = position; }
    void SetLightingTexture(Texture2D* lighting) { m_lighting = lighting; }
    
    /**
     * Render to window
     */
    void Render(Scene* scene);
    
protected:
    /**
     * Initialize shader
     */
    void InitializeShader();
    
protected:
    // Pointers to textures from main render
    Texture2D* m_diffuse;
    Texture2D* m_normal;
    Texture2D* m_position;
    Texture2D* m_lighting;
};

#endif
