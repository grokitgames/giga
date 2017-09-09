
#include <giga-engine.h>

void StartApplication();

GIGA_APPLICATION(StartApplication);

void StartApplication() {
    // Get application
    Application* application = Application::GetInstance();
    
    // Register systems
    EntitySystem* entitySystem = application->CreateSystem<EntitySystem>();
	ReplicationSystem* replicationSystem = application->CreateSystem<ReplicationSystem>();
    EventSystem* eventSystem = application->CreateSystem<EventSystem>();
    ScriptingSystem* scriptingSystem = application->CreateSystem<ScriptingSystem>(20);
    InputSystem* inputSystem = application->CreateSystem<InputSystem>(20);
    ErrorSystem* errorSystem = application->CreateSystem<ErrorSystem>();
    ResourceSystem* resourceSystem = application->CreateSystem<ResourceSystem>();
    OpenGLRenderSystem* renderSystem = application->CreateSystem<OpenGLRenderSystem>();
    MaterialSystem* materialSystem = application->CreateSystem<MaterialSystem>();
    NetworkSystem* networkSystem = application->CreateSystem<NetworkSystem>();
    
    application->Initialize();
    
    // Create a new window
    Window* window = new Window();
    window->Create("Game", 800, 600, false);
    
    // Set the primary window (in case we need it elsewhere)
    application->SetPrimaryWindow(window);
    
    // Get framebuffer size (for retina mostly)
    int width = window->GetFramebufferWidth();
    int height = window->GetFramebufferHeight();
    
    // Set up resource system
    resourceSystem->AddSearchPath("./");
    resourceSystem->AddSearchPath("Resources/Meshes");
    resourceSystem->AddSearchPath("Resources/Textures");
    resourceSystem->AddSearchPath("Resources/Shaders");
	resourceSystem->AddSearchPath("Resources/Scripts/Client");
    
    // Initialize render system
    renderSystem->Initialize(width, height);
    
    // Load game.js file
    Script* gamejs = dynamic_cast<Script*>(resourceSystem->LoadResource("game.js", "Script"));
    ScriptComponent* gameComponent = new ScriptComponent();
    
    // Call Init() inside JS file
    gameComponent->AddToSystem();
    gameComponent->Initialize(gamejs);
    
    // Set camera as active
    Scene* scene = renderSystem->GetScene();
    scene->SetAmbientLight(vector3(1, 1, 1));
    
    // Create keyboard
    Keyboard* keyboard = new Keyboard();
    keyboard->Initialize();
    inputSystem->RegisterInputDevice(keyboard);
	
	// Connect to server
	networkSystem->Connect("grokitgames.com", 8053);

	// Initialize replication system
	replicationSystem->SetType(REPLICATION_CLIENT);
    
    // Create main loop timer
    Timer* mainTimer = new Timer();
    mainTimer->Start();
    float delta = 0.0f;

    // Main loop
    while(window->IsClosing() == false) {
        PROFILE_START_FRAME();

        application->Update(delta);
        
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
}
