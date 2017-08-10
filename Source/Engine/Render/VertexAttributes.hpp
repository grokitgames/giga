
#ifndef vertexattributes_h
#define vertexattributes_h

/**
 * Vertex attribute object used in OpenGL for collective attribute setting
 */
class GIGA_API VertexAttributes {
public:
    VertexAttributes() = default;
    virtual ~VertexAttributes() = default;
    
    /**
     * Create a new vertex attrib object
     */
    virtual void Create() { }
    
    /**
     * Use these vertex attributes
     */
    virtual void Bind()  { }
    
    /**
     * Unbind
     */
    virtual void Unbind() { }
    
    /**
     * Destroy the vertex attrib object
     */
    virtual void Destroy() { }
    
    /**
     * Enable a vertex attribute
     */
    virtual void EnableAttribute(unsigned int index, unsigned int size, unsigned int stride, unsigned int offset) { }
    
    /**
     * Disable a vertex attribute
     */
    virtual void DisableAttribute(unsigned int index)  { }
};

#endif 
