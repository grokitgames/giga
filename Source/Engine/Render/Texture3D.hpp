
#ifndef texture3d_hpp
#define texture3d_hpp

/**
 * Our class for storing 3D (cubemap) textures
 */
class GIGA_API Texture3D : public Texture {
public:
    Texture3D() = default;
    ~Texture3D() = default;
    
    /**
     * Initialize a new "zero" texture
     */
    void Initialize(int width, int height, int format, int type);
    
    /**
     * Bind/unbind this texture to a texture slot
     */
    void Bind(int slot);
    void Unbind();
};

#endif
