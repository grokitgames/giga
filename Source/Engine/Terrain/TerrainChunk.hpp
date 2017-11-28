
#ifndef terrainchunk_hpp
#define terrainchunk_hpp

/**
 * Lazy-loaded (optional), quad-tree divided chunks of terrain
 */
class GIGA_API TerrainChunk : public StaticMeshComponent {
public:
    TerrainChunk();
    ~TerrainChunk();
    
    GIGA_CLASS_NAME("TerrainChunk");
    
    /**
     * Set up a terrain chunk (loads heightmap file, but not data into buffers)
     */
    void Initialize(std::string heightmapFilename, float x, float z);

    /**
     * Load the heightmap into texture meshes
     */
    void Load(int destQuadSize, int currentQuadSize, float x, float z, float scale, unsigned char* pixels);
    
protected:
    // The heightmap texture
    Texture2D* m_heightmap;
    
    // The filename of the heightmap texture
    std::string m_heightmapFilename;
    
    // Diffuse texture base
    Texture2D* m_diffuseTexture;
    
    // The offset x and z values for this chunk
    float m_x, m_z;
    
    // Splat map
    Texture2D* m_splat;
};

#endif
