
#include <giga-engine.h>
#include <Render/OpenGL.hpp>

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
    
    // Set up vertex buffer
    m_buffer = new VertexBuffer();
    m_buffer->Create(4, 4, box, false);
    
    m_format->AddVertexAttribute(VERTEX_ATTRIB_POSITION, 2, 4, 0);
    m_format->AddVertexAttribute(VERTEX_ATTRIB_TEXCOORD0, 2, 4, 2);
}

void PostProcessPass::Initialize(int windowWidth, int windowHeight) {
    Framebuffer* framebuffer = new Framebuffer();
    framebuffer->Initialize(windowWidth, windowHeight);
    
    // Create our textures
    Texture2D* texture = new Texture2D();
    texture->Initialize(windowWidth, windowHeight, GL_RGB16F, GL_RGB);
    framebuffer->AddTexture(texture, GL_COLOR_ATTACHMENT0);
    
    m_framebuffers.push_back(framebuffer);
    
    InitializeShader();
    CreateVertexFormat(windowWidth, windowHeight);
}

Texture2D* PostProcessPass::GetOutputTexture() {
    return(m_framebuffers[0]->GetTexture(0));
}
