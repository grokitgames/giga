
#ifndef spotlightcomponent_hpp
#define spotlightcomponent_hpp

class GIGA_API SpotLightComponent : public LightComponent {
public:
    SpotLightComponent() { m_type = LIGHT_SPOT; }
    ~SpotLightComponent() = default;
    
    GIGA_CLASS_NAME("SpotLightCompoent");
    
    /**
     * Initialize
     */
    void Initialize();
    
    /**
     * Create depth textures
     */
    void CreateDepthTextures(Scene* scene);
    
    /**
     * Clone spotlight
     */
    SpotLightComponent* Clone();
    
protected:
    // Re-calc matrices
    void RecalculateMatrices();
};

#endif
