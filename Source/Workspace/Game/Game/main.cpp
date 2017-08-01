
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
    resourceSystem->AddSearchPath("Resources/Scripts");
    
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
    cc->SetWorldPosition(vector3(0, 1.25, 3));
    cc->SetLookVector(vector3(0, -0.1, -1.0));
    camera->AddComponent(cc);
    
    // Set camera as active
    Scene* scene = renderSystem->GetScene();
    scene->SetActiveCamera(cc);
    scene->SetAmbientLight(vector3(1, 1, 1));
    
    // Add script to camera
    ScriptComponent* sc = dynamic_cast<ScriptComponent*>(Component::CreateComponent("ScriptComponent"));
    Script* inputjs = dynamic_cast<Script*>(resourceSystem->LoadResource("input.js", "Script"));
    sc->Initialize(inputjs);
    
    camera->AddComponent(sc);
    
    // Get other necessary systems
    InputSystem* inputSystem = GetSystem<InputSystem>();
    ScriptingSystem* scriptingSystem = GetSystem<ScriptingSystem>();
    EventSystem* eventSystem = GetSystem<EventSystem>();
    
    // Create keyboard
    Keyboard* keyboard = new Keyboard();
    keyboard->Initialize();
    inputSystem->RegisterInputDevice(keyboard);
    
    // Create main loop timer
    Timer* mainTimer = new Timer();
    mainTimer->Start();
    float delta = 0.1f;
    
    // Main loop
    while(window->IsClosing() == false) {
        PROFILE_START_FRAME();
        
        scriptingSystem->Update(delta);
        renderSystem->Update(delta);
        eventSystem->Update(delta);
        
        window->SwapBuffer();
        window->ProcessEvents();
        
        delta = mainTimer->Duration();
        mainTimer->Reset();
        
        PROFILE_END_FRAME();
        Timer::Sleep(1);
    }
    
    // Dump profiler
    Profiler* profiler = Profiler::GetInstance();
    profiler->Log();
    
    application->Shutdown();
    
    return 0;
}
