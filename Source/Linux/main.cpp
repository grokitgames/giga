
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
	RenderSystem* renderSystem = application->CreateSystem<RenderSystem>();
	MaterialSystem* materialSystem = application->CreateSystem<MaterialSystem>();
	NetworkSystem* networkSystem = application->CreateSystem<NetworkSystem>();
	ReplicationSystem* replicationSystem = application->CreateSystem<ReplicationSystem>();

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
	crateScript->Initialize(cratejs);
	crateScript->SetActive(true);
	crateScript->AddToSystem();

	crate->AddComponent(crateScript);

	// Set up server
	networkSystem->Listen(8053);

	// Set up replication system
	replicationSystem->SetType(REPLICATION_SERVER);

	// Create main loop timer
	Timer* mainTimer = new Timer();
	mainTimer->Start();
	float delta = 0.1f;

	// Main loop
	while (true) {
		PROFILE_START_FRAME();

		application->Update(delta);

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
