
#ifndef vertexbuffer_h
#define vertexbuffer_h

/**
 * GPU vertex buffer
 */
class GIGA_API VertexBuffer {
public:
    VertexBuffer() : m_count(0), m_vertexSize(0), m_dynamic(false) { }
    virtual ~VertexBuffer() = default;
    
    /**
     * Create the buffer
     */
    virtual void Create(int count, int vertexSize, void* data, bool dynamic) { }
    
    /**
     * Set new data (ideally for streaming buffers - full replace)
     */
    virtual void SetData(void* data) { }
    
    /**
     * Read the buffer's contents back out
     */
    virtual float* GetData(int& count) { assert(false); return(0); }
    
    /**
     * Get the number of vertices inside the buffer
     */
    int GetVertexCount() { return m_count; }
    
    /**
     * Get the size of the vertices
     */
    int GetVertexSize() { return m_vertexSize; }
    
    /**
     * Use this vertex buffer for rendering
     */
    virtual void Bind() { }
    
    /**
     * Finish using this vertex buffer
     */
    virtual void Unbind() { }
    
    /**
     * Destroy the buffer
     */
    virtual void Destroy() { }
    
protected:
    // Count of vertices stored in buffer
    int m_count;
    
    // Size of each vertex
    int m_vertexSize;
    
    // Whether this buffer is streaming or static
    bool m_dynamic;
};


#endif
