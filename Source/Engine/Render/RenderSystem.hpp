
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
     * Scripting integration
     */
    static Variant* GetWindowWidth(Variant* obj, int argc, Variant** argv);
    static Variant* GetWindowHeight(Variant* obj, int argc, Variant** argv);
    
protected:
    int m_windowWidth, m_windowHeight;
};

#endif
