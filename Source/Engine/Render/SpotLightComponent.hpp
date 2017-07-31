
#ifndef spotlightcomponent_hpp
#define spotlightcomponent_hpp

class GIGA_API SpotLightComponent : public LightComponent {
public:
    SpotLightComponent() { m_type = LIGHT_SPOT; }
    ~SpotLightComponent() = default;
    
    /**
     * Initialize
     */
    void Initialize();
    
    /**
     * Create depth textures
     */
    void CreateDepthTextures(Scene* scene);
};

#endif
