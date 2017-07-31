
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
    
    // Initialize render system
    RenderSystem* renderSystem = GetSystem<RenderSystem>();
    renderSystem->Initialize(width, height);
    
    // Create an entity
    Entity* entity = new Entity();
    
    // Load a mesh
    ResourceSystem* resourceSystem = GetSystem<ResourceSystem>();
    Mesh* mesh = dynamic_cast<Mesh*>(resourceSystem->FindResource("Resources/Meshes/crate.g3d"));
    StaticMeshComponent* meshComponent = dynamic_cast<StaticMeshComponent*>(Component::CreateComponent("StaticMeshComponent"));
    meshComponent->Instantiate(mesh);

    // Add to entity
    entity->AddComponent(meshComponent);
    
    // Main loop
    while(window->IsClosing() == false) {
        window->SwapBuffer();
        window->ProcessEvents();
        
        Timer::Sleep(1);
    }
    
    application->Shutdown();
    
    return 0;
}
