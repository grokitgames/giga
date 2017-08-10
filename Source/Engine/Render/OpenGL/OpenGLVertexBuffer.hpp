
#ifndef openglvertexbuffer_hpp
#define openglvertexbuffer_hpp

/**
 * GPU vertex buffer
 */
class GIGA_API OpenGLVertexBuffer : public VertexBuffer {
public:
    OpenGLVertexBuffer();
    ~OpenGLVertexBuffer();
    
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
    // Internal buffer ID
    unsigned int m_buffer;
};

#endif
