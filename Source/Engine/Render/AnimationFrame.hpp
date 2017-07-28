
#ifndef animationframe_hpp
#define animationframe_hpp

/**
 * A frame of an animation
 */
class GIGA_API AnimationFrame {
public:
    AnimationFrame();
    ~AnimationFrame();
    
public:
    /**
     * The timestamp of this transformation (in ticks)
     */
    uint32_t time;
    
    /**
     * The objects affected by this and how
     */
    std::vector<AnimationTransform*> transforms;
};

#endif
