
#include <giga-engine.h>

void StartApplication();

GIGA_APPLICATION(StartApplication);

void StartApplication() {
    // Get application
    Application* application = Application::GetInstance();
	    
    // Register systems
    EntitySystem* entitySystem = application->CreateSystem<EntitySystem>();
	EventSystem* eventSystem = application->CreateSystem<EventSystem>(20);
	ReplicationSystem* replicationSystem = application->CreateSystem<ReplicationSystem>();
	ScriptingSystem* scriptingSystem = application->CreateSystem<ScriptingSystem>(20);
    InputSystem* inputSystem = application->CreateSystem<InputSystem>(20);
    ErrorSystem* errorSystem = application->CreateSystem<ErrorSystem>();
    ResourceSystem* resourceSystem = application->CreateSystem<ResourceSystem>();
    MaterialSystem* materialSystem = application->CreateSystem<MaterialSystem>();
    NetworkSystem* networkSystem = application->CreateSystem<NetworkSystem>();
	TaskSystem* taskSystem = application->CreateSystem<TaskSystem>();
    TerrainSystem* terrainSystem = application->CreateSystem<TerrainSystem>();
    OpenGLRenderSystem* renderSystem = application->CreateSystem<OpenGLRenderSystem>();
    
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

	// Set camera as active
	Scene* scene = renderSystem->GetScene();
	scene->SetAmbientLight(vector3(0.3, 0.3, 0.3));
    
	// Create keyboard
	Keyboard* keyboard = new Keyboard();
	keyboard->Initialize();
	inputSystem->RegisterInputDevice(keyboard);

	// Initialize replication system
	replicationSystem->SetType(REPLICATION_CLIENT);

    scriptingSystem->Lock(scriptingSystem);
    
    // Load game.js file
    Script* gamejs = dynamic_cast<Script*>(resourceSystem->LoadResource("game.js", "Script"));
    ScriptComponent* gameComponent = new ScriptComponent();
    
    // Call Init() inside JS file
    gameComponent->AddToSystem();
    gameComponent->Initialize(gamejs);

	// Load client.js file
	Script* clientjs = dynamic_cast<Script*>(resourceSystem->LoadResource("client.js", "Script"));
	ScriptComponent* clientComponent = new ScriptComponent();

	// Call Init() inside JS file
	clientComponent->AddToSystem();
    clientComponent->Initialize(clientjs);

	// Create some stuff
    Entity* crate = entitySystem->CreateEntity("Crate");
    
    Mesh* crateMesh = (Mesh*)resourceSystem->LoadResource("crate.g3d", "Mesh");
    StaticMeshComponent* meshComponent = new StaticMeshComponent();
    crate->AddComponent(meshComponent);
    meshComponent->AddToSystem();
    meshComponent->SetActive(true);
    
    meshComponent->Instantiate(crateMesh);
    meshComponent->SetWorldPosition(vector3(0, 0, 0));
    meshComponent->SetWorldRotation(quaternion(1, 0.00, 0, 0.00));
    
    Script* crateScript = (Script*)resourceSystem->LoadResource("crate.js", "Script");
    ScriptComponent* scriptComponent = new ScriptComponent();
    
    crate->AddComponent(scriptComponent);
    scriptComponent->AddToSystem();
    scriptComponent->Initialize(crateScript);
    scriptComponent->SetActive(true);
    
    scriptingSystem->Unlock();
    
    Entity* floor = entitySystem->CreateEntity("Floor");
    
    Mesh* floorMesh = (Mesh*)resourceSystem->LoadResource("floor.g3d", "Mesh");
    StaticMeshComponent* floorComponent = new StaticMeshComponent();
    floor->AddComponent(floorComponent);
    floorComponent->AddToSystem();
    floorComponent->SetActive(true);
    
    floorComponent->Instantiate(floorMesh);
    floorComponent->SetWorldPosition(vector3(0, 0, 0));
    floorComponent->SetWorldRotation(quaternion(1, 0.00, 0, 0.00));
    
    Entity* light = entitySystem->CreateEntity("Light");
    SpotLightComponent* spotLight = new SpotLightComponent();
    spotLight->SetWorldPosition(vector3(0, 3, 2));
    spotLight->SetColor(vector3(1, 1, 1));
    spotLight->SetAttenuation(20.0f);
    spotLight->Initialize();
    
    spotLight->AddToSystem();
    spotLight->SetActive(true);
    
    // Terrain
    terrainSystem->LoadChunk("terrain1.png", 0, 0, 0.25f);
    
    // Create main loop timer
    Timer* mainTimer = new Timer();
    mainTimer->Start();
    float delta = 0.0f;

    // Main loop
    while(window->IsClosing() == false) {
        PROFILE_START_FRAME();

		delta = mainTimer->Duration();
		mainTimer->Reset();
		//delta = remainderf(delta, 0.05);

        application->Update(delta);
        
        window->SwapBuffer();
        window->ProcessEvents();
        
        PROFILE_END_FRAME();
        Timer::Sleep(1);
    }
    
    // Dump profiler
    Profiler* profiler = Profiler::GetInstance();
    profiler->Log();
    
    application->Shutdown();
}
