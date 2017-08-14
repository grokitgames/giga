
#ifndef rendersystem_hpp
#define rendersystem_hpp

/**
 * Our rendering system, processing various renderable components
 */
class GIGA_API RenderSystem : public System {
public:
    RenderSystem();
    virtual ~RenderSystem() = default;
    
	GIGA_CLASS_NAME("RenderSystem");
    
    /**
     * Initialize with width/height
     */
    void Initialize();
    
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
	virtual VertexBuffer* CreateVertexBuffer() { return(new VertexBuffer()); }
    virtual IndexBuffer* CreateIndexBuffer() { return(new IndexBuffer()); }
    virtual VertexAttributes* CreateVertexAttributes() { return(new VertexAttributes()); }
    virtual Texture2D* CreateTexture2D() { return(new Texture2D()); }
    virtual Texture3D* CreateTexture3D() { return(new Texture3D()); }
    virtual ShaderVariation* CreateShaderVariation() { return(new ShaderVariation()); }
    virtual Framebuffer* CreateFramebuffer() { return(new Framebuffer()); }
    virtual ShadowPass* CreateShadowPass() { return(new ShadowPass()); }
    
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
