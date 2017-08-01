
#ifndef gbufferpass_hpp
#define gbufferpass_hpp

class GIGA_API GBufferPass : public RenderPass {
public:
    GBufferPass() = default;
    ~GBufferPass() = default;
    
    /**
     * Initialize Gbuffer
     */
    void Initialize(int windowWidth, int windowHeight);
    
    /**
     * Render to Gbuffer
     */
    void Render(Scene* scene);
    
    /**
     * Get textures
     */
    Texture2D* GetDiffuseTexture();
    Texture2D* GetNormalTexture();
    Texture2D* GetPositionTexture();
    Texture2D* GetDepthTexture();
    
private:
    // Internally recurse through the children of a mesh component
    void RecursiveRender(StaticMeshComponent* mesh, matrix4 parent, Scene* scene, int index);
    
    // Storage for combined view/proj matrix
    matrix4 m_viewproj;
    matrix4 m_view;
    
    // Window width and height
    int m_windowWidth, m_windowHeight;
};

#endif
