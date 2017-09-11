
#include <giga-engine.h>

void StartApplication();

GIGA_APPLICATION(StartApplication);

void StartApplication() {
	// Get application
	Application* application = Application::GetInstance();

	// Register systems
	EntitySystem* entitySystem = application->CreateSystem<EntitySystem>(20);
	EventSystem* eventSystem = application->CreateSystem<EventSystem>(20);
	ReplicationSystem* replicationSystem = application->CreateSystem<ReplicationSystem>(20);
	ScriptingSystem* scriptingSystem = application->CreateSystem<ScriptingSystem>(20);
	InputSystem* inputSystem = application->CreateSystem<InputSystem>(20);
	ErrorSystem* errorSystem = application->CreateSystem<ErrorSystem>(20);
	ResourceSystem* resourceSystem = application->CreateSystem<ResourceSystem>(20);
	RenderSystem* renderSystem = application->CreateSystem<RenderSystem>(20);
	MaterialSystem* materialSystem = application->CreateSystem<MaterialSystem>(20);
	NetworkSystem* networkSystem = application->CreateSystem<NetworkSystem>();

	application->Initialize();

	// Set up resource system
    resourceSystem->AddSearchPath("./");
	resourceSystem->AddSearchPath("Resources/Meshes");
	resourceSystem->AddSearchPath("Resources/Textures");
	resourceSystem->AddSearchPath("Resources/Shaders");
	resourceSystem->AddSearchPath("Resources/Scripts/Server");
    
    // Load game.js file
    Script* gamejs = dynamic_cast<Script*>(resourceSystem->LoadResource("game.js", "Script"));
    ScriptComponent* gameComponent = new ScriptComponent();
    
    // Call Init() inside JS file
    gameComponent->AddToSystem();
    gameComponent->Initialize(gamejs);

	// Create a crate entity
	Entity* crate = entitySystem->CreateEntity("Crate");

	// Load a mesh
	Mesh* mesh = dynamic_cast<Mesh*>(resourceSystem->LoadResource("crate.g3d", "Mesh"));
	StaticMeshComponent* meshComponent = dynamic_cast<StaticMeshComponent*>(Component::CreateComponent("StaticMeshComponent"));
	meshComponent->Instantiate(mesh);
	meshComponent->SetActive(true);
	meshComponent->AddToSystem();

	// Add to entity
	crate->AddComponent(meshComponent);

	// Add script to crate
	ScriptComponent* crateScript = dynamic_cast<ScriptComponent*>(Component::CreateComponent("ScriptComponent"));
	Script* cratejs = dynamic_cast<Script*>(resourceSystem->LoadResource("crate.js", "Script"));
	crate->AddComponent(crateScript);

	crateScript->Initialize(cratejs);
	crateScript->SetActive(true);
	crateScript->AddToSystem();

	// Set up server
	networkSystem->Listen(8053);

	// Set up replication system
	replicationSystem->SetType(REPLICATION_SERVER);

	// Create main loop timer
	Timer* mainTimer = new Timer();
	mainTimer->Start();
	float delta = 0.0f;
	
	// Main loop
	while (true) {
		delta = mainTimer->Duration();
		mainTimer->Reset();

		PROFILE_START_FRAME();

		application->Update(delta);

		PROFILE_END_FRAME();
		Timer::Sleep(1);
	}

	// Dump profiler
	Profiler* profiler = Profiler::GetInstance();
	profiler->Log();

	application->Shutdown();
}
