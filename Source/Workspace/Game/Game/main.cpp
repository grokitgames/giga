
#include <giga-engine.h>

int main(int argc, const char * argv[]) {
    // Get application
    Application* application = Application::GetInstance();
    application->Initialize("REEVE");
    
    // Create a new window
    Window* window = new Window();
    window->Create("Game", 800, 600, false);
    
    // Set the primary window (in case we need it elsewhere)
    application->SetPrimaryWindow(window);
    
    // Get framebuffer size (for retina mostly)
    int width = window->GetFramebufferWidth();
    int height = window->GetFramebufferHeight();
    
    // Set up resource system
    ResourceSystem* resourceSystem = GetSystem<ResourceSystem>();
    resourceSystem->AddSearchPath("Resources/Meshes");
    resourceSystem->AddSearchPath("Resources/Textures");
    resourceSystem->AddSearchPath("Resources/Shaders");
    
    // Initialize render system
    RenderSystem* renderSystem = GetSystem<RenderSystem>();
    renderSystem->Initialize(width, height);
    
    // Create a crate entity
    Entity* crate = new Entity();
    
    // Load a mesh
    Mesh* mesh = dynamic_cast<Mesh*>(resourceSystem->LoadResource("crate.g3d", "Mesh"));
    StaticMeshComponent* meshComponent = dynamic_cast<StaticMeshComponent*>(Component::CreateComponent("StaticMeshComponent"));
    meshComponent->Instantiate(mesh);

    // Add to entity
    crate->AddComponent(meshComponent);
    
    // Create our camera
    Entity* camera = new Entity();

    // Create a camera
    CameraComponent* cc = dynamic_cast<CameraComponent*>(Component::CreateComponent("CameraComponent"));
    cc->SetViewport(width, height);
    camera->AddComponent(cc);
    
    // Set camera as active
    Scene* scene = renderSystem->GetScene();
    scene->SetActiveCamera(cc);
    scene->SetAmbientLight(vector3(1, 1, 1));
    
    // Main loop
    while(window->IsClosing() == false) {
        PROFILE_START_FRAME();
        
        renderSystem->Update(0.0f);
        
        window->SwapBuffer();
        window->ProcessEvents();
        
        PROFILE_END_FRAME();
        Timer::Sleep(10);
    }
    
    // Dump profiler
    Profiler* profiler = Profiler::GetInstance();
    profiler->Log();
    
    application->Shutdown();
    
    return 0;
}
