
#ifndef texture2d_hpp
#define texture2d_hpp

/**
 * Our class for storing textures loaded from disk
 */
class GIGA_API Texture2D : public ResourceObject {
public:
    Texture2D();
    virtual ~Texture2D() = default;
    
    /**
     * Initialize a new "zero" texture
     */
    virtual void Initialize(int width, int height, int format, int channels);
    
    /**
     * Process data loaded into a resource
     */
    void ProcessData();
    
    /**
     * Bind this texture to a texture slot
     */
    virtual void Bind(int slot);
    
    /**
     * Unbind this texture
     */
    virtual void Unbind();
    
    /**
     * Dump texture to disk
     */
    virtual void Save(std::string filename);
    
    /**
     * Get texture info
     */
    int GetWidth() { return m_width; }
    int GetHeight() { return m_height; }
    int GetChannels() { return m_channels; }
    
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
