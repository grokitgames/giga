
#ifndef renderpass_hpp
#define renderpass_hpp

class Scene;

class GIGA_API RenderPass {
public:
    RenderPass() = default;
    virtual ~RenderPass();
    
    /**
     * Initialize render pass
     */
    virtual void Initialize(int windowWidth, int windowHeight);
    
    /**
     * Render our scene
     */
    virtual void Render(Scene* scene) = 0;
    
    /**
     * Get a framebuffer
     */
    Framebuffer* GetFramebuffer(int index);
    
protected:
    // Framebuffers attached to this render pass
    std::vector<Framebuffer*> m_framebuffers;
    
    // An overriding shader (if applicable)
    ShaderVariation* m_shader;
    
    // How many triangles this pass rendered
    int m_renderedTriangles;
    
    // How many draw calls this pass required
    int m_drawCalls;
    
    // Window width and height
    int m_windowWidth, m_windowHeight;
};

#endif
