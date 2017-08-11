
#ifndef opengltexture_hpp
#define opengltexture_hpp

class GIGA_API OpenGLTexture {
public:
    OpenGLTexture() : m_texture(0), m_slot(0) { }
    virtual ~OpenGLTexture() = default;
    
protected:
    // Internal texture ID
    unsigned int m_texture;
    
    // Bound texture slot
    int m_slot;
};

#endif
