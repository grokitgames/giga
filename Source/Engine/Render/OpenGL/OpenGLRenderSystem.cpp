
#include <giga-engine.h>
#include <Render/OpenGL/OpenGL.hpp>

OpenGLRenderSystem::OpenGLRenderSystem() {
    m_gbufferPass = 0;
    m_deferredPass = 0;
    m_lightingPass = 0;
}

void OpenGLRenderSystem::Initialize(int width, int height) {
    // Save
    m_windowWidth = width;
    m_windowHeight = height;
    
    // Create scene
    if(m_scene == 0) {
        m_scene = new Scene();
    }
    
    // Initialize GBuffer
    if(m_gbufferPass) {
        delete m_gbufferPass;
    }
    
    m_gbufferPass = new OpenGLGBufferPass();
    m_gbufferPass->Initialize(width, height);
    
    // Lighting pass
    if(m_lightingPass) {
        delete m_lightingPass;
    }
    
    m_lightingPass = new OpenGLDeferredLightingPass();
    m_lightingPass->Initialize(width, height);
    m_lightingPass->SetDiffuseTexture(m_gbufferPass->GetDiffuseTexture());
    m_lightingPass->SetNormalTexture(m_gbufferPass->GetNormalTexture());
    m_lightingPass->SetPositionTexture(m_gbufferPass->GetPositionTexture());
    
    // Initialize deferred render buffer
    if(m_deferredPass) {
        delete m_deferredPass;
    }
    
    m_deferredPass = new OpenGLDeferredRenderPass();
    m_deferredPass->Initialize(width, height);
    m_deferredPass->SetDiffuseTexture(m_gbufferPass->GetDiffuseTexture());
    m_deferredPass->SetNormalTexture(m_gbufferPass->GetNormalTexture());
    m_deferredPass->SetPositionTexture(m_gbufferPass->GetPositionTexture());
    m_deferredPass->SetLightingTexture(m_lightingPass->GetLightingTexture());
    
    // Log some stuff
    const unsigned char* version = glGetString(GL_VERSION);
    const unsigned char* driver = glGetString(GL_RENDERER);
    const unsigned char* glsl = glGetString(GL_SHADING_LANGUAGE_VERSION);
    
    Application::Log(MSGTYPE_INFO, "OpenGL Version", (char*)version);
    Application::Log(MSGTYPE_INFO, "Renderer", (char*)driver);
    Application::Log(MSGTYPE_INFO, "GLSL Version", (char*)glsl);
    
    /**
     * OpenGL specific component types
     */
    ResourceSystem* resourceSystem = GetSystem<ResourceSystem>();
    resourceSystem->RegisterResourceType<OpenGLTexture2D>("Texture2D");
}

void OpenGLRenderSystem::Update(float delta) {
	PROFILE_START_AREA("RenderSystem Update");

    // Depth first pass
    
    // Gbuffer pass
    m_gbufferPass->Render(m_scene);
    
    // Lighting pass
    m_lightingPass->Render(m_scene);
    
    // Render to screen
    m_deferredPass->Render(m_scene);
    
    // Un-lit objects / debug data / billboards / skybox
    
    // Post processing passes

	PROFILE_END_AREA("RenderSystem Update");
}

VertexBuffer* OpenGLRenderSystem::CreateVertexBuffer() {
    return(new OpenGLVertexBuffer());
}

IndexBuffer* OpenGLRenderSystem::CreateIndexBuffer() {
    return(new OpenGLIndexBuffer());
}

VertexAttributes* OpenGLRenderSystem::CreateVertexAttributes() {
    return(new OpenGLVertexAttributes());
}

Texture2D* OpenGLRenderSystem::CreateTexture2D() {
    return(new OpenGLTexture2D());
}

Texture3D* OpenGLRenderSystem::CreateTexture3D() {
    return(new OpenGLTexture3D());
}

ShaderVariation* OpenGLRenderSystem::CreateShaderVariation() {
    return(new OpenGLShaderVariation());
}

Framebuffer* OpenGLRenderSystem::CreateFramebuffer() {
    return(new OpenGLFramebuffer());
}

ShadowPass* OpenGLRenderSystem::CreateShadowPass() {
    return(new OpenGLShadowPass());
}
