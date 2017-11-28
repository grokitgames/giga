
#include <giga-engine.h>

TerrainChunk::TerrainChunk() {
    m_heightmap = 0;
    m_splat = 0;
    m_x = m_z = 0.0f;
}

TerrainChunk::~TerrainChunk() {
    if(m_heightmap) {
        delete m_heightmap;
        m_heightmap = 0;
    }
}

void TerrainChunk::Initialize(std::string heightmapFilename, float x, float z) {
    // Save heightmap file
    m_heightmapFilename = heightmapFilename;
    
    // Save offset coords
    m_x = x;
    m_z = z;
    
    // Load heightmap texture
    ResourceSystem* resourceSystem = GetSystem<ResourceSystem>();
    m_heightmap = (Texture2D*)resourceSystem->LoadResource(heightmapFilename, "Texture2D");
    
    // Check for a splat map
    std::string extension = m_heightmap->GetResource()->GetExtension();
    std::string prefix = heightmapFilename.substr(0, heightmapFilename.length() - extension.length() - 1);
    std::string splatFilename = prefix + "_splat." + extension;
    std::string splatFilePath = resourceSystem->FindResourcePath(splatFilename);
    
    if(splatFilePath.empty() == false) {
        m_splat = (Texture2D*)resourceSystem->LoadResource(splatFilename, "Texture2D");
    }
    
    std::string diffuseFilename = prefix + "_diffuse." + extension;
    std::string diffuseFilePath = resourceSystem->FindResourcePath(diffuseFilename);
    
    if(diffuseFilePath.empty() == false) {
        m_diffuseTexture = (Texture2D*)resourceSystem->LoadResource(diffuseFilename, "Texture2D");
    }
}

void TerrainChunk::Load(int destQuadSize, int currentQuadSize, float x, float z, float scale, unsigned char* pixels) {
    if(pixels == 0) {
        // Top level - start descending
        pixels = (unsigned char*)m_heightmap->GetData();
    }
    
    if(currentQuadSize == 0) {
        currentQuadSize = m_heightmap->GetWidth() - 1;
    }
    
    // Divide into quadSize + 1 chunks
    if(destQuadSize < currentQuadSize) {
        int nextQuadSize = currentQuadSize / 2;
        for(int nz = z; nz < (z + currentQuadSize); nz += nextQuadSize) {
            for(int nx = x; nx < (x + currentQuadSize); nx += nextQuadSize) {
                TerrainChunk* child = new TerrainChunk();
                child->m_heightmap = m_heightmap;
                child->m_diffuseTexture = m_diffuseTexture;
                child->Load(destQuadSize, nextQuadSize, nx, nz, scale, pixels);
                
                m_children.push_back(child);
            }
        }
        
        return;
    }
    
    // Get render system
    RenderSystem* renderSystem = GetSystem<RenderSystem>();
    MaterialSystem* materialSystem = GetSystem<MaterialSystem>();
    
    // Save offsets
    m_x = x;
    m_z = z;
    
    int orig_width = m_heightmap->GetWidth();
    int orig_height = m_heightmap->GetHeight();
    int channels = m_heightmap->GetChannels();
    
    // Internal vertex format: position (3) + tex0 (2) + tex1 (2)
    // Normals in normal map
    int vertexSize = 7;
    
    // Force internal data storage to a single unsigned byte per pixel
    // NOTE: if multi-channel, this will only take the first (ie. red)
    std::vector<float> newData;
    newData.resize((currentQuadSize + 1) * (currentQuadSize + 1) * vertexSize);

    // Copy data
    for(int nz = 0; nz <= currentQuadSize; nz++) {
        int currentOrigOffset = (((z + nz) * orig_width) + x) * channels;
        
        for(int nx = 0; nx <= currentQuadSize; nx++) {
            int currentNewOffset = ((nz * (currentQuadSize + 1)) + nx) * vertexSize;
            
            // Position
            newData[currentNewOffset + 0] = (m_x + (float)nx) * scale;
            newData[currentNewOffset + 1] = pixels[currentOrigOffset + (nx * channels)] - 128;
            newData[currentNewOffset + 2] = (m_z + (float)nz) * scale;
            
            // Tex 0 (full map texture)
            newData[currentNewOffset + 3] = ((float)x + nx) / (float)orig_width;
            newData[currentNewOffset + 4] = ((float)z + nz) / (float)orig_height;
            
            // Tex 1 (local - detail texture)
            newData[currentNewOffset + 5] = ((float)nx) / (float)currentQuadSize;
            newData[currentNewOffset + 6] = ((float)nz) / (float)currentQuadSize;
        }
    }
    
    // Create mesh
    TerrainMesh* mesh = new TerrainMesh();
    
    mesh->m_diffuseTexture = m_diffuseTexture;
    mesh->m_material = materialSystem->GetDefaultMaterial();
    
    // Create vertex format
    mesh->m_vertexFormat = new VertexFormat();
    mesh->m_vertexFormat->Bind();
    
    mesh->m_vertexFormat->AddVertexAttribute(VERTEX_ATTRIB_POSITION, 3, vertexSize, 0);
    mesh->m_vertexFormat->AddVertexAttribute(VERTEX_ATTRIB_TEXCOORD0, 2, vertexSize, 3);
    mesh->m_vertexFormat->AddVertexAttribute(VERTEX_ATTRIB_TEXCOORD1, 2, vertexSize, 5);
    
    // Create vertex buffer
    mesh->m_vertexBuffer = renderSystem->CreateVertexBuffer();
    mesh->m_vertexBuffer->Create((currentQuadSize + 1) * (currentQuadSize + 1), vertexSize, newData.data(), false);
    
    // Create indices
    std::vector<unsigned int> indices;
    indices.resize((currentQuadSize + 1) * (currentQuadSize + 1) * 3 * 2);
    
    for(int nz = 0; nz < currentQuadSize; nz++) {
        for(int nx = 0; nx < currentQuadSize; nx++) {
            int offset = ((nz * (currentQuadSize + 1)) + nx) * 3 * 2;
            
            // Triangle 1
            indices[offset + 0] = ((nz * (currentQuadSize + 1)) + nx);
            indices[offset + 1] = (((nz + 1) * (currentQuadSize + 1)) + nx);
            indices[offset + 2] = ((nz * (currentQuadSize + 1)) + (nx + 1));
            
            // Triangle 2
            indices[offset + 3] = ((nz * (currentQuadSize + 1)) + (nx + 1));
            indices[offset + 4] = (((nz + 1) * (currentQuadSize + 1)) + nx);
            indices[offset + 5] = (((nz + 1) * (currentQuadSize + 1)) + (nx + 1));
        }
    }
    
    mesh->m_indexBuffer = renderSystem->CreateIndexBuffer();
    mesh->m_indexBuffer->Create((currentQuadSize + 1) * (currentQuadSize + 1) * 3 * 2, indices.data());
    
    // Set properties
    mesh->m_numTriangles = (currentQuadSize + 1) * (currentQuadSize + 1) * 2;
    mesh->m_shader = ShaderSystem::Find("terrain.vs", "terrain.fs", new ShaderAttributes());
    
    mesh->m_aabb.Create(vector3(0), vector3(currentQuadSize));
    
    // Set offset from origin
    m_transform.position = vector3(x, 0, z);
    
    // Save mesh
    this->Instantiate(mesh);
}
