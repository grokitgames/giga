
#ifndef mesh_hpp
#define mesh_hpp

/**
 * Basic mesh storage class
 */
class GIGA_API Mesh : public ResourceObject {
public:
    Mesh();
    ~Mesh();

	GIGA_CLASS_NAME("Mesh");
    
    /**
     * Load mesh data
     */
    void ProcessData();
    
    /**
     * Getters
     */
    VertexBuffer* GetVertexBuffer() { return m_vertexBuffer; }
    IndexBuffer* GetIndexBuffer() { return m_indexBuffer; }
    VertexFormat* GetVertexFormat() { return m_vertexFormat; }
    Texture2D* GetDiffuseTexture() { return m_diffuseTexture; }
    Texture2D* GetNormalTexture() { return m_normalTexture; }
    Material* GetMaterial() { return m_material; }
    ShaderVariation* GetShader() { return m_shader; }
    BoundingBox& GetBoundingBox() { return m_aabb; }
    int GetTriCount() { return m_numTriangles; }
    
    /**
     * Get children
     */
    std::vector<Mesh*>& GetChildren() { return m_children; }
    
    /**
     * Mesh file format sections
     */
    enum {
        SECTION_MESH = 1,
        SECTION_BONES,
        SECTION_ANIMATIONS,
        SECTION_LAST
    };
    
protected:
    /**
     * Get the min/max vertices to create AABB
     */
    void GetMinMaxAABB(vector3& min, vector3& max);
    
protected:
    // Child objects
    std::vector<Mesh*> m_children;
    
    // Vertex buffer
    VertexBuffer* m_vertexBuffer;
    
    // Index buffer
    IndexBuffer* m_indexBuffer;
    
    // Vertex format
    VertexFormat* m_vertexFormat;
    
    // Diffuse texture
    Texture2D* m_diffuseTexture;
    
    // Additional (optional) textures
    Texture2D* m_normalTexture;
    
    // Material
    Material* m_material;
    
    // Shader program to render our object
    ShaderVariation* m_shader;
    
    // Number of triangles (must be set for rendering)
    int m_numTriangles;
    
    // Bounding box of vertices (min and max)
    BoundingBox m_aabb;
    
    // Bones
    std::vector<Bone*> m_bones;
    
    // Skeletal animations
    std::vector<Animation*> m_animations;
};

#endif
