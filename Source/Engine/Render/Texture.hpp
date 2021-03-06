
#ifndef texture_hpp
#define texture_hpp

class GIGA_API Texture : public ResourceObject {
public:
    Texture() : m_width(0), m_height(0), m_channels(0) { }
    virtual ~Texture() = default;
    
    /**
     * Initialize a new "zero" texture
     */
    virtual void Initialize(int width, int height, int format, int type) { }
    
    /**
     * Set data
     */
    virtual void SetData(int width, int height, int format, int type, void* data) { }
    
    /**
     * Bind this texture to a texture slot
     */
    virtual void Bind(int slot) { }
    
    /**
     * Unbind this texture
     */
    virtual void Unbind() { }
    
    /**
     * Process data loaded into a resource
     */
    virtual void ProcessData() { }
    
    /**
     * Dump texture to disk
     */
    virtual void Save(std::string filename) { }
    
    /**
     * Get texture info
     */
    int GetWidth() { return m_width; }
    int GetHeight() { return m_height; }
    int GetChannels() { return m_channels; }
	virtual unsigned int GetTexture() { return(0); }
    virtual unsigned int GetTarget(int slot) { return(0); }
    
    /**
     * Get texture data
     */
    virtual void* GetData() { return(0); }
    
protected:
    // Width and height
    int m_width, m_height;
    
    // Number of channels
    int m_channels;
};

#endif
