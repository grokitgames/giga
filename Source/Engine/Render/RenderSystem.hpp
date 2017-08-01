
#ifndef rendersystem_hpp
#define rendersystem_hpp

/**
 * Our rendering system, processing various renderable components
 */
class GIGA_API RenderSystem : public System {
public:
    RenderSystem();
    ~RenderSystem() = default;
    
    GIGA_CLASS_NAME("RenderSystem")
    
    /**
     * Initialize with width/height
     */
    void Initialize(int width, int height);
    
    /**
     * Update (render)
     */
    void Update(float delta);
    
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
    static Component* CreateStaticMeshComponent();
    static void RemoveStaticMeshComponent(Component* component);
    
    /**
     * Camera creation/removal
     */
    static Component* CreateCameraComponent();
    static void RemoveCameraComponent(Component* component);
    
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
    
    // Orthographic projection (for deferred rendering/post-processing)
    matrix4 m_ortho;
    
    // Built-in system passes
    GBufferPass* m_gbufferPass;
    DeferredRenderPass* m_deferredPass;
    DeferredLightingPass* m_lightingPass;
    
    // Render passes
    ObjectPool<RenderPass> m_passes;
};

#endif
