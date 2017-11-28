
#ifndef terrainsystem_hpp
#define terrainsystem_hpp

class GIGA_API TerrainSystem : public System {
public:
    TerrainSystem() = default;
    ~TerrainSystem() = default;
    
    GIGA_CLASS_NAME("TerrainSystem");
    
    /**
     * Update any loaded/unloaded terrain chunks based on a camera view
     */
    void Update(float delta);
    
    /**
     * Load chunk manually from heightmap
     */
    void LoadChunk(std::string heightmap, float x, float z, float scale);
    
protected:
    // Current camera (for streaming terrain)
    CameraComponent* m_camera;
    
    // List of terrain chunks
    ObjectPool<TerrainChunk> m_terrainChunks;
};

#endif
