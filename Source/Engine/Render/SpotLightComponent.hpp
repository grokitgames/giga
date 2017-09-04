
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
     * Copy spotlight
     */
	void Copy(Component* component);
    
protected:
    struct LookVectors {
        vector3 look;
        vector3 up;
    };
    
protected:
    // Re-calc matrices
    void RecalculateMatrices();
    
    // Set look vectors
    LookVectors m_vectors[6];
};

#endif
