
#ifndef framebuffer_hpp
#define framebuffer_hpp

/**
 * Single framebuffer object to which multiple render targets can be attached
 */
class GIGA_API Framebuffer {
public:
    Framebuffer();
    ~Framebuffer();
    
    /**
     * Initialize (will detroy any existing framebuffer and textures)
     */
    void Initialize(int width, int height);
    
    /**
     * Attach a texture to our framebuffer
     */
    void AddTexture(Texture2D* texture, int type);
    
    /**
     * Set a texture to a specific slot (one-time, not permanent)
     */
    void SetTexture(Texture2D* texture, int type);
    
    /** 
     * Get a texture back out
     */
    Texture2D* GetTexture(int index) { return m_textures[index]; }
    
    /**
     * Use this framebuffer for rendering/output
     */
    void Use();
    
    /**
     * Getters
     */
    int GetWidth() { return m_width; }
    int GetHeight() { return m_height; }
    
protected:
    // The textures attached to this framebuffer
    std::vector<Texture2D*> m_textures;
    
    // The color/output slots this framebuffer draws to
    std::vector<unsigned int> m_slots;
    
    // Our internal framebuffer ID
    unsigned int m_framebuffer;
    
    // Whether this framebuffer has been checked for completeness
    bool m_complete;
    
    // Width and height (for viewport)
    int m_width, m_height;
};

#endif
