
#ifndef framebuffer_hpp
#define framebuffer_hpp

extern int FRAMEBUFFER_SLOT_0;
extern int FRAMEBUFFER_SLOT_1;
extern int FRAMEBUFFER_SLOT_2;
extern int FRAMEBUFFER_SLOT_3;
extern int FRAMEBUFFER_SLOT_4;
extern int FRAMEBUFFER_SLOT_5;
extern int FRAMEBUFFER_SLOT_DEPTH;

/**
 * Single framebuffer object to which multiple render targets can be attached
 */
class GIGA_API Framebuffer {
public:
    Framebuffer();
    virtual ~Framebuffer();
    
    /**
     * Initialize (will detroy any existing framebuffer and textures)
     */
    virtual void Initialize(int width, int height) { }
    
    /**
     * Attach a texture to our framebuffer
     */
    virtual void AddTexture(Texture2D* texture, int type) { }
    
    /**
     * Set a texture to a specific slot (one-time, not permanent)
     */
    virtual void SetTexture(Texture2D* texture, int type) { }
    
    /**
     * Set a texture to a specific slot (one-time, not permanent)
     */
    virtual void SetTexture(Texture3D* texture, int type, int slot) { }
    
    /** 
     * Get a texture back out
     */
    Texture2D* GetTexture(int index) { return m_textures[index]; }
    
    /**
     * Use this framebuffer for rendering/output
     */
    virtual void Use() { }
    
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
    
    // Whether this framebuffer has been checked for completeness
    bool m_complete;
    
    // Width and height (for viewport)
    int m_width, m_height;
};

#endif
