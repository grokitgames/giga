
#include <giga-engine.h>

void StartApplication();

GIGA_APPLICATION(StartApplication);

void StartApplication() {
    // Get application
    Application* application = Application::GetInstance();
    
    // Register systems
    EntitySystem* entitySystem = application->CreateSystem<EntitySystem>();
    EventSystem* eventSystem = application->CreateSystem<EventSystem>();
    ScriptingSystem* scriptingSystem = application->CreateSystem<ScriptingSystem>();
    InputSystem* inputSystem = application->CreateSystem<InputSystem>();
    ErrorSystem* errorSystem = application->CreateSystem<ErrorSystem>();
    ResourceSystem* resourceSystem = application->CreateSystem<ResourceSystem>();
    OpenGLRenderSystem* renderSystem = application->CreateSystem<OpenGLRenderSystem>();
    MaterialSystem* materialSystem = application->CreateSystem<MaterialSystem>();
    NetworkSystem* networkSystem = application->CreateSystem<NetworkSystem>();
	ReplicationSystem* replicationSystem = application->CreateSystem<ReplicationSystem>();
    
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
    
    // Create our camera
    Entity* camera = new Entity();

    // Create a camera
    CameraComponent* cc = dynamic_cast<CameraComponent*>(Component::CreateComponent("CameraComponent"));
    camera->AddComponent(cc);
    
    cc->SetViewport(width, height);
    cc->SetWorldPosition(vector3(0, 1.25, 3));
    cc->SetLookVector(vector3(0, -0.1, -1.0));
    
    // Set camera as active
    Scene* scene = renderSystem->GetScene();
    scene->SetActiveCamera(cc);
    scene->SetAmbientLight(vector3(1, 1, 1));
    
    // Create keyboard
    Keyboard* keyboard = new Keyboard();
    keyboard->Initialize();
    inputSystem->RegisterInputDevice(keyboard);

    // Add script to camera
    ScriptComponent* cameraScript = dynamic_cast<ScriptComponent*>(Component::CreateComponent("ScriptComponent"));
    Script* inputjs = dynamic_cast<Script*>(resourceSystem->LoadResource("camera.js", "Script"));
	cameraScript->AddToSystem();
	cameraScript->Initialize(inputjs);
	cameraScript->SetActive(true);
    
    camera->AddComponent(cameraScript);
	
	// Connect to server
	networkSystem->Connect("grokitgames.com", 8053);

	// Initialize replication system
	replicationSystem->SetType(REPLICATION_CLIENT);
    
    // Create main loop timer
    Timer* mainTimer = new Timer();
    mainTimer->Start();
    float delta = 0.1f;
    
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
