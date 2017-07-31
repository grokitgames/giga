
#ifndef vertexarrayobject_hpp
#define vertexarrayobject_hpp

/**
 * Vertex attribute object used in OpenGL for collective attribute setting
 */
class GIGA_API VertexAttribObject {
public:
    VertexAttribObject() : m_vertexArray(0) { };
    ~VertexAttribObject();
    
    /**
     * Create a new vertex attrib object
     */
    void Create();
    
    /**
     * Use these vertex attributes
     */
    void Bind();
    
    /**
     * Destroy the vertex attrib object
     */
    void Destroy();
    
    /**
     * Enable a vertex attribute
     */
    void EnableVertexAttribute(unsigned int index, unsigned int size, unsigned int stride, unsigned int offset);
    
    /**
     * Disable a vertex attribute
     */
    void DisableVertexAttribute(unsigned int index);
    
protected:
    // Internal storage ID
    unsigned int m_vertexArray;
};

#endif
