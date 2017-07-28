
#ifndef vertexbuffer_hpp
#define vertexbuffer_hpp

/**
 * GPU vertex buffer
 */
class GIGA_API VertexBuffer {
public:
    VertexBuffer();
    ~VertexBuffer();
    
    /**
     * Create the buffer
     */
    void Create(int count, int vertexSize, void* data, bool dynamic);
    
    /**
     * Set new data (ideally for streaming buffers - full replace)
     */
    void SetData(void* data);
    
    /**
     * Read the buffer's contents back out
     */
    float* GetData(int& count);
    
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
    void Bind();
    
    /**
     * Finish using this vertex buffer
     */
    void Unbind();
    
    /**
     * Destroy the buffer
     */
    void Destroy();
    
protected:
    // Count of vertices stored in buffer
    int m_count;
    
    // Internal buffer ID
    unsigned int m_buffer;
    
    // Size of each vertex
    int m_vertexSize;
    
    // Whether this buffer is streaming or static
    bool m_dynamic;
};

#endif
