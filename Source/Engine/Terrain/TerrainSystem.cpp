
#include <giga-engine.h>

#define TERRAIN_QUAD_SIZE       64

void TerrainSystem::Update(float delta) {
    
}

void TerrainSystem::LoadChunk(std::string heightmap, float x, float z, float scale) {
    // Load recursively to a destination quad size
    TerrainChunk* chunk = new TerrainChunk();
    chunk->Initialize(heightmap, x, z);
    chunk->Load(TERRAIN_QUAD_SIZE, 0, x, z, scale, 0);
    
    // Add chunk for tracking
    m_terrainChunks.AddObject(chunk);
    
    // Activate chunk
    chunk->SetActive(true);
    
    // Add to render system
    RenderSystem* renderSystem = GetSystem<RenderSystem>();
    renderSystem->AddStaticMeshComponent(chunk);
}
