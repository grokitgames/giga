
#include <giga-engine.h>

RenderSystem::RenderSystem() {
    m_windowWidth = m_windowHeight = 0;
    m_scene = 0;
}

void RenderSystem::Initialize() {
	if (m_scene) {
		delete m_scene;
	}

	m_scene = new Scene();
}

void RenderSystem::AddRenderPass(RenderPass* pass) {
    m_passes.AddObject(pass);
}

void RenderSystem::RemoveRenderPass(RenderPass* pass) {
    m_passes.RemoveObject(pass);
}

void RenderSystem::AddStaticMeshComponent(Component* component) {
    RenderSystem* renderSystem = GetSystem<RenderSystem>();
	StaticMeshComponent* mesh = (StaticMeshComponent*)component;
    renderSystem->m_scene->m_meshes.AddObject(mesh);
}

void RenderSystem::RemoveStaticMeshComponent(Component* component) {
    RenderSystem* renderSystem = GetSystem<RenderSystem>();
    renderSystem->m_scene->m_meshes.RemoveObject((StaticMeshComponent*)component);
}

void RenderSystem::AddCameraComponent(Component* component) {
    RenderSystem* renderSystem = GetSystem<RenderSystem>();
	CameraComponent* camera = (CameraComponent*)component;
    renderSystem->m_scene->m_cameras.AddObject(camera);
}

void RenderSystem::RemoveCameraComponent(Component* component) {
    RenderSystem* renderSystem = GetSystem<RenderSystem>();
    renderSystem->m_scene->m_cameras.RemoveObject((CameraComponent*)component);
}

void RenderSystem::AddSpotLightComponent(Component* component) {
    RenderSystem* renderSystem = GetSystem<RenderSystem>();
	SpotLightComponent* light = (SpotLightComponent*)component;
    renderSystem->m_scene->m_lights.AddObject(light);
}

void RenderSystem::RemoveSpotLightComponent(Component* component) {
    RenderSystem* renderSystem = GetSystem<RenderSystem>();
    renderSystem->m_scene->m_lights.RemoveObject((SpotLightComponent*)component);
}

Variant* RenderSystem::GetWindowWidth(Variant* obj, int argc, Variant** argv) {
    RenderSystem* renderSystem = GetSystem<RenderSystem>();
    return(new Variant(renderSystem->m_windowWidth));
}

Variant* RenderSystem::GetWindowHeight(Variant* obj, int argc, Variant** argv) {
    RenderSystem* renderSystem = GetSystem<RenderSystem>();
    return(new Variant(renderSystem->m_windowHeight));
}

Variant* RenderSystem::SetActiveCamera(Variant* obj, int argc, Variant** argv) {
	GIGA_ASSERT(argc == 1, "SetActiveCamera expects one argument.");
	GIGA_ASSERT(argv[0]->IsObject(), "First argument should be CameraComponent.");

	RenderSystem* renderSystem = GetSystem<RenderSystem>();
	renderSystem->m_scene->SetActiveCamera(argv[0]->AsObject<CameraComponent>());
	return(new Variant(0));
}