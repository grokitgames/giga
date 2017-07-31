
#ifndef texture_hpp
#define texture_hpp

class GIGA_API Texture {
public:
    Texture();
    virtual ~Texture() = default;
    
    /**
     * Initialize a new "zero" texture
     */
    virtual void Initialize(int width, int height, int format, int type) = 0;
    
    /**
     * Bind this texture to a texture slot
     */
    virtual void Bind(int slot) = 0;
    
    /**
     * Unbind this texture
     */
    virtual void Unbind() = 0;
    
    /**
     * Get texture info
     */
    int GetWidth() { return m_width; }
    int GetHeight() { return m_height; }
    int GetChannels() { return m_channels; }
    unsigned int GetTexture() { return m_texture; }
    
protected:
    // Internal texture ID
    unsigned int m_texture;
    
    // Width and height
    int m_width, m_height;
    
    // Number of channels
    int m_channels;
    
    // Bound texture slot
    int m_slot;
};

#endif
