
#ifndef postprocesspass_hpp
#define postprocesspass_hpp

class GIGA_API PostProcessPass : public RenderPass {
public:
    PostProcessPass();
    ~PostProcessPass() = default;
    
    /**
     * Set ortho output
     */
    void CreateVertexFormat(int windowWidth, int windowHeight);
    
    /**
     * Initialize (create a single output)
     */
    virtual void Initialize(int windowWidth, int windowHeight);
    
    /**
     * Set input texture
     */
    virtual void SetInputTexture(Texture2D* texture) { m_input = texture; }
    
    /**
     * Get output texture (overridable, default is framebuffer 0, texture 0)
     */
    virtual Texture2D* GetOutputTexture();
    
protected:
    /**
     * Create your own shader
     */
    virtual void InitializeShader() = 0;
    
protected:
    // Ortho screen matrix
    matrix4 m_ortho;
    
    // Vertex format
    VertexFormat* m_format;
    
    // Vertex buffer
    VertexBuffer* m_buffer;
    
    // Input texture
    Texture2D* m_input;
};

#endif
