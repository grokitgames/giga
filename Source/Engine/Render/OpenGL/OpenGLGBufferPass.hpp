
#ifndef openglgbufferpass_hpp
#define openglgbufferpass_hpp

class GIGA_API OpenGLGBufferPass : public RenderPass {
public:
    OpenGLGBufferPass() = default;
    ~OpenGLGBufferPass() = default;
    
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
    Texture2D* GetMaterialTexture();
    
private:
    // Internally recurse through the children of a mesh component
    void RecursiveRender(StaticMeshComponent* mesh, matrix4 parent, Scene* scene, int index);
    
    // Storage for combined view/proj matrix
    matrix4 m_viewproj;
    matrix4 m_view;
};

#endif
