
#ifndef openglrendersystem_hpp
#define openglrendersystem_hpp

/**
 * Our rendering system, processing various renderable components
 */
class GIGA_API OpenGLRenderSystem : public RenderSystem {
public:
    OpenGLRenderSystem();
    ~OpenGLRenderSystem() = default;
    
    /**
     * Initialize with width/height
     */
    void Initialize(int width, int height);
    
    /**
     * Update (render)
     */
    void Update(float delta);
    
    /**
     * Create objects
     */
    VertexBuffer* CreateVertexBuffer();
    IndexBuffer* CreateIndexBuffer();
    VertexAttributes* CreateVertexAttributes();
    Texture2D* CreateTexture2D();
    Texture3D* CreateTexture3D();
    ShaderVariation* CreateShaderVariation();
    Framebuffer* CreateFramebuffer();
    ShadowPass* CreateShadowPass();
    
protected:
    // Orthographic projection (for deferred rendering/post-processing)
    matrix4 m_ortho;
    
    // Built-in system passes
    OpenGLGBufferPass* m_gbufferPass;
    OpenGLDeferredRenderPass* m_deferredPass;
    OpenGLDeferredLightingPass* m_lightingPass;
};

#endif 
