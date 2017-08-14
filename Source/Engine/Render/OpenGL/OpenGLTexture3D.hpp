
#ifndef opengltexture3d_hpp
#define opengltexture3d_hpp

/**
 * Our class for storing 3D (cubemap) textures
 */
class GIGA_API OpenGLTexture3D : public Texture3D, public OpenGLTexture {
public:
    OpenGLTexture3D() = default;
    virtual ~OpenGLTexture3D() = default;
    
    /**
     * Initialize a new "zero" texture
     */
    void Initialize(int width, int height, int format, int type);
    
    /**
     * Set data
     */
    void SetData(int width, int height, int format, int type, void* data) { assert(false); }
    
    /**
     * Get texture info
     */
    unsigned int GetTexture() { return m_texture; }
    
    /**
     * Bind/unbind this texture to a texture slot
     */
    void Bind(int slot);
    void Unbind();
    
    /**
     * Get target (always GL_TEXTURE_2D for 2D textures)
     */
    unsigned int GetTarget(int slot);
};

#endif
