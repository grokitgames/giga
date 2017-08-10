
#ifndef openglvertexattributes_hpp
#define openglvertexattributes_hpp

/**
 * Vertex attribute object used in OpenGL for collective attribute setting
 */
class GIGA_API OpenGLVertexAttributes : public VertexAttributes {
public:
    OpenGLVertexAttributes() : m_vertexArray(0) { };
    ~OpenGLVertexAttributes();
    
    /**
     * Create a new vertex attrib object
     */
    void Create();
    
    /**
     * Use these vertex attributes
     */
    void Bind();
    
    /**
     * Unbind
     */
    void Unbind();
    
    /**
     * Destroy the vertex attrib object
     */
    void Destroy();
    
    /**
     * Enable a vertex attribute
     */
    void EnableAttribute(unsigned int index, unsigned int size, unsigned int stride, unsigned int offset);
    
    /**
     * Disable a vertex attribute
     */
    void DisableAttribute(unsigned int index);
    
protected:
    // Internal storage ID
    unsigned int m_vertexArray;
};

#endif
