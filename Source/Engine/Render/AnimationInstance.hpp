
#ifndef animationinstance_hpp
#define animationinstance_hpp

class GIGA_API AnimationInstance {
public:
    AnimationInstance() : m_time(0.0f) { }
    ~AnimationInstance() = default;
    
    /**
     * Update the animation
     */
    virtual void Update(float delta) { }
    
protected:
    // Current running time into animation
    float m_time;
};

#endif
