
#ifndef opengltexture2d_hpp
#define opengltexture2d_hpp

/**
 * Our class for storing textures loaded from disk
 */
class GIGA_API OpenGLTexture2D : public OpenGLTexture, public Texture2D {
public:
    OpenGLTexture2D() = default;
    ~OpenGLTexture2D() = default;
    
    /**
     * Initialize a new "zero" texture
     */
    void Initialize(int width, int height, int format, int type);
    
    /**
     * Set data
     */
    void SetData(int width, int height, int format, int type, void* data);
    
    /**
     * Bind/unbind this texture to a texture slot
     */
    void Bind(int slot);
    void Unbind();
    
    /**
     * Get texture info
     */
    unsigned int GetTexture() { return m_texture; }
    
    /**
     * Process data loaded into a resource
     */
    void ProcessData();
    
    /**
     * Dump texture to disk
     */
    void Save(std::string filename);
    
    /**
     * Get target (always GL_TEXTURE_2D for 2D textures)
     */
    unsigned int GetTarget(int slot);
};

#endif 
