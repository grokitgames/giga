
#include <giga-engine.h>
#include <Render/OpenGL.hpp>

RenderSystem::RenderSystem() {
    m_windowWidth = m_windowHeight = 0;
    m_scene = 0;
    m_gbufferPass = 0;
    m_deferredPass = 0;
}

void RenderSystem::Initialize(int width, int height) {
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
    
    m_gbufferPass = new GBufferPass();
    m_gbufferPass->Initialize(width, height);
    
    // Lighting pass
    if(m_lightingPass) {
        delete m_lightingPass;
    }
    
    m_lightingPass = new DeferredLightingPass();
    m_lightingPass->Initialize(width, height);
    m_lightingPass->SetDiffuseTexture(m_gbufferPass->GetDiffuseTexture());
    m_lightingPass->SetNormalTexture(m_gbufferPass->GetNormalTexture());
    m_lightingPass->SetPositionTexture(m_gbufferPass->GetPositionTexture());
    
    // Initialize deferred render buffer
    if(m_deferredPass) {
        delete m_deferredPass;
    }
    
    m_deferredPass = new DeferredRenderPass();
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
}

void RenderSystem::Update(float delta) {
    // Depth first pass
    
    // Gbuffer pass
    m_gbufferPass->Render(m_scene);
    
    // Lighting pass
    m_lightingPass->Render(m_scene);
    
    // Render to screen
    m_deferredPass->Render(m_scene);
    
    // Un-lit objects / debug data / billboards / skybox
    
    // Post processing passes
}

void RenderSystem::AddRenderPass(RenderPass* pass) {
    m_passes.AddObject(pass);
}

void RenderSystem::RemoveRenderPass(RenderPass* pass) {
    m_passes.RemoveObject(pass);
}

Component* RenderSystem::CreateStaticMeshComponent() {
    RenderSystem* renderSystem = GetSystem<RenderSystem>();
    
    StaticMeshComponent* mesh = new StaticMeshComponent();
    renderSystem->m_scene->m_meshes.AddObject(mesh);
    return(mesh);
}

void RenderSystem::RemoveStaticMeshComponent(Component* component) {
    RenderSystem* renderSystem = GetSystem<RenderSystem>();
    renderSystem->m_scene->m_meshes.RemoveObject(dynamic_cast<StaticMeshComponent*>(component));
}

Component* RenderSystem::CreateCameraComponent() {
    RenderSystem* renderSystem = GetSystem<RenderSystem>();
    
    CameraComponent* camera = new CameraComponent();
    renderSystem->m_scene->m_cameras.AddObject(camera);
    return(camera);
}

void RenderSystem::RemoveCameraComponent(Component* component) {
    RenderSystem* renderSystem = GetSystem<RenderSystem>();
    renderSystem->m_scene->m_cameras.RemoveObject(dynamic_cast<CameraComponent*>(component));
}

Variant* RenderSystem::GetWindowWidth(Variant* obj, int argc, Variant** argv) {
    RenderSystem* renderSystem = GetSystem<RenderSystem>();
    return(new Variant(renderSystem->m_windowWidth));
}

Variant* RenderSystem::GetWindowHeight(Variant* obj, int argc, Variant** argv) {
    RenderSystem* renderSystem = GetSystem<RenderSystem>();
    return(new Variant(renderSystem->m_windowHeight));
}
