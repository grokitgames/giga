
#ifndef rendersystem_hpp
#define rendersystem_hpp

/**
 * Our rendering system, processing various renderable components
 */
class GIGA_API RenderSystem : public System {
public:
    RenderSystem();
    virtual ~RenderSystem() = default;
    
    GIGA_CLASS_NAME("RenderSystem")
    
    /**
     * Initialize with width/height
     */
    virtual void Initialize(int width, int height) { }
    
    /**
     * Update (render)
     */
    virtual void Update(float delta) { }
    
    /**
     * Add or remove a post-process filter
     */
    void AddRenderPass(RenderPass* pass);
    void RemoveRenderPass(RenderPass* pass);
    
    /**
     * Get current scene
     */
    Scene* GetScene() { return m_scene; }
    
    /**
     * Static mesh creation/removal
     */
    static Component* CreateStaticMeshComponent(std::string type);
    static void RemoveStaticMeshComponent(Component* component);
    
    /**
     * Camera creation/removal
     */
    static Component* CreateCameraComponent(std::string type);
    static void RemoveCameraComponent(Component* component);
    
    /**
     * Spot light creation/removal
     */
    static Component* CreateSpotLightComponent(std::string type);
    static void RemoveSpotLightComponent(Component* component);
    
    /**
     * Create objects
     */
	virtual VertexBuffer* CreateVertexBuffer() { return(0); }
    virtual IndexBuffer* CreateIndexBuffer() { return(0); }
    virtual VertexAttributes* CreateVertexAttributes() { return(0); }
    virtual Texture2D* CreateTexture2D() { return(0); }
    virtual Texture3D* CreateTexture3D() { return(0); }
    virtual ShaderVariation* CreateShaderVariation() { return(0); }
    virtual Framebuffer* CreateFramebuffer() { return(0); }
    virtual ShadowPass* CreateShadowPass() { return(0); }
    
    /**
     * Scripting integration
     */
    static Variant* GetWindowWidth(Variant* obj, int argc, Variant** argv);
    static Variant* GetWindowHeight(Variant* obj, int argc, Variant** argv);
    
protected:
    // Current window (framebuffer) width/height
    int m_windowWidth, m_windowHeight;
    
    // Current scene
    Scene* m_scene;
    
    // Render passes
    ObjectPool<RenderPass> m_passes;
};

#endif
