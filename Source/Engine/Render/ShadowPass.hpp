
#ifndef shadowpass_hpp
#define shadowpass_hpp

class ShadowPass : public RenderPass {
public:
    ShadowPass() = default;
    ~ShadowPass() = default;
    
    /**
     * Set frustum
     */
    void SetFrustum(Frustum* frustum) { m_frustum = frustum; }
    
    /**
     * Set position
     */
    void SetPosition(vector3 position) { m_position = position; }

	/**
	 * Render
	 */
	virtual void Render(Scene* scene) { }

protected:
    // Frustum
    Frustum* m_frustum;
    
    // Position of light
    vector3 m_position;
};

#endif
