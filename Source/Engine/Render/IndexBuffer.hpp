
#ifndef indexbuffer_hpp
#define indexbuffer_hpp

/**
 * GPU index buffer
 */
class GIGA_API IndexBuffer {
public:
    IndexBuffer();
    ~IndexBuffer();
    
    /**
     * Create a new buffer and fill it
     */
    void Create(int count, void* data);
    
    /**
     * Get the count of indices in this buffer
     */
    int GetIndexCount() { return m_count; }
    
    /**
     * Use this index buffer
     */
    void Use();
    
    /**
     * Destroy this buffer's contents
     */
    void Destroy();
    
    /**
     * Read the buffer's contents
     */
    unsigned int* GetData(int& count);
    
protected:
    // Count of indices
    int m_count;
    
    // Internal buffer ID
    unsigned int m_buffer;
};

#endif
