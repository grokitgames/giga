
#include <giga-engine.h>

PostProcessPass::PostProcessPass() {
    m_format = 0;
    m_buffer = 0;
    m_input = 0;
}

void PostProcessPass::CreateVertexFormat(int windowWidth, int windowHeight) {
    // Create our ortho screen matrix
    m_ortho = glm::ortho(0.0f, (float)windowWidth, (float)windowHeight, 0.0f);
    
    // Set up vertices
    float box[] = {
        (float)windowWidth, 0, 1, 1,
        0, 0, 0, 1,
        (float)windowWidth, (float)windowHeight, 1, 0,
        0, (float)windowHeight, 0, 0,
    };
    
    // Use our vertex shader
    GIGA_ASSERT(m_shader != 0, "Shader must be set up before vertex format is created.");
    m_shader->Bind();
    
    // Set up our vertex format
    m_format = new VertexFormat();
    m_format->Bind();
    
    // Get render system
    RenderSystem* renderSystem = GetSystem<RenderSystem>();
    
    // Set up vertex buffer
    m_buffer = renderSystem->CreateVertexBuffer();
    m_buffer->Create(4, 4, box, false);
    
    m_format->AddVertexAttribute(VERTEX_ATTRIB_POSITION, 2, 4, 0);
    m_format->AddVertexAttribute(VERTEX_ATTRIB_TEXCOORD0, 2, 4, 2);
}

void PostProcessPass::Initialize(int windowWidth, int windowHeight) {
    RenderSystem* renderSystem = GetSystem<RenderSystem>();
    Framebuffer* framebuffer = renderSystem->CreateFramebuffer();
    framebuffer->Initialize(windowWidth, windowHeight);
    
    // Create our textures
    Texture2D* texture = renderSystem->CreateTexture2D();
    texture->Initialize(windowWidth, windowHeight, COLOR_RGB16F, COLOR_RGB);
    framebuffer->AddTexture(texture, FRAMEBUFFER_SLOT_0);
    
    m_framebuffers.push_back(framebuffer);
    
    InitializeShader();
    CreateVertexFormat(windowWidth, windowHeight);
    
    m_windowWidth = windowWidth;
    m_windowHeight = windowHeight;
}

Texture2D* PostProcessPass::GetOutputTexture() {
    return((Texture2D*)m_framebuffers[0]->GetTexture(0));
}
