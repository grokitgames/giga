
#ifndef texture2d_hpp
#define texture2d_hpp

/**
 * Our class for storing textures loaded from disk
 */
class GIGA_API Texture2D : public ResourceObject, public Texture {
public:
    Texture2D() = default;
    ~Texture2D() = default;
    
    /**
     * Initialize a new "zero" texture
     */
    void Initialize(int width, int height, int format, int type);
    
    /**
     * Bind/unbind this texture to a texture slot
     */
    void Bind(int slot);
    void Unbind();
    
    /**
     * Process data loaded into a resource
     */
    void ProcessData();
    
    /**
     * Dump texture to disk
     */
    virtual void Save(std::string filename);
};

#endif
