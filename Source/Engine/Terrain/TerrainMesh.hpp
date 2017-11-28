
#ifndef terrainmesh_hpp
#define terrainmesh_hpp

class TerrainChunk;

class GIGA_API TerrainMesh : public Mesh {
public:
    TerrainMesh() = default;
    ~TerrainMesh() = default;
    
    friend class TerrainChunk;
};

#endif
