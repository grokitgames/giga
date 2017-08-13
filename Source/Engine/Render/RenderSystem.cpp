
#include <giga-engine.h>

RenderSystem::RenderSystem() {
    m_windowWidth = m_windowHeight = 0;
    m_scene = 0;
}

void RenderSystem::AddRenderPass(RenderPass* pass) {
    m_passes.AddObject(pass);
}

void RenderSystem::RemoveRenderPass(RenderPass* pass) {
    m_passes.RemoveObject(pass);
}

Component* RenderSystem::CreateStaticMeshComponent(std::string type) {
    RenderSystem* renderSystem = GetSystem<RenderSystem>();
    
    StaticMeshComponent* mesh = new StaticMeshComponent();
    renderSystem->m_scene->m_meshes.AddObject(mesh);
    return(mesh);
}

void RenderSystem::RemoveStaticMeshComponent(Component* component) {
    RenderSystem* renderSystem = GetSystem<RenderSystem>();
    renderSystem->m_scene->m_meshes.RemoveObject(dynamic_cast<StaticMeshComponent*>(component));
}

Component* RenderSystem::CreateCameraComponent(std::string type) {
    RenderSystem* renderSystem = GetSystem<RenderSystem>();
    
    CameraComponent* camera = new CameraComponent();
    renderSystem->m_scene->m_cameras.AddObject(camera);
    return(camera);
}

void RenderSystem::RemoveCameraComponent(Component* component) {
    RenderSystem* renderSystem = GetSystem<RenderSystem>();
    renderSystem->m_scene->m_cameras.RemoveObject(dynamic_cast<CameraComponent*>(component));
}

Component* RenderSystem::CreateSpotLightComponent(std::string type) {
    RenderSystem* renderSystem = GetSystem<RenderSystem>();
    
    SpotLightComponent* light = new SpotLightComponent();
    renderSystem->m_scene->m_lights.AddObject(light);
    return(light);
}

void RenderSystem::RemoveSpotLightComponent(Component* component) {
    RenderSystem* renderSystem = GetSystem<RenderSystem>();
    renderSystem->m_scene->m_lights.RemoveObject(dynamic_cast<SpotLightComponent*>(component));
}

Variant* RenderSystem::GetWindowWidth(Variant* obj, int argc, Variant** argv) {
    RenderSystem* renderSystem = GetSystem<RenderSystem>();
    return(new Variant(renderSystem->m_windowWidth));
}

Variant* RenderSystem::GetWindowHeight(Variant* obj, int argc, Variant** argv) {
    RenderSystem* renderSystem = GetSystem<RenderSystem>();
    return(new Variant(renderSystem->m_windowHeight));
}
