
#ifndef shadowpass_hpp
#define shadowpass_hpp

class ShadowPass : public RenderPass {
public:
    ShadowPass() = default;
    ~ShadowPass() = default;
    
    /**
     * Look at
     */
    void SetView(CameraComponent* camera) { m_camera = camera; }

	/**
	 * Render
	 */
	virtual void Render(Scene* scene) { }

protected:
    // View
    CameraComponent* m_camera;
};

#endif
