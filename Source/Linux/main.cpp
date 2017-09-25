
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
	TaskSystem* taskSystem = application->CreateSystem<TaskSystem>();

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

	// Create our data loader
	MySQLDataLoader* loader = application->CreateDataLoader<MySQLDataLoader>();
	loader->Connect("localhost", "mmorpg", "password01!", "mmorpg");

	// Load all entities in scene
	std::vector<StorableObject*> entities = loader->GetRecords("Entity", 1);
	for (int i = 0; i < entities.size(); i++) {
		// Add entity to system
		entitySystem->AddEntity((Entity*)entities[i]);
	}

	// Get and add all component types
	std::vector<std::string> componentTypes = Component::GetComponentTypes();
	for (int i = 0; i < componentTypes.size(); i++) {
		StorableObjectType* type = DataLoader::GetRecordType(componentTypes[i]);
		if (type == 0) {
			continue;
		}

		std::vector<StorableObject*> components = loader->GetRecords(componentTypes[i], 1);

		for (int j = 0; j < components.size(); j++) {
			// Find entity
			Entity* entity = entitySystem->FindEntity(atoi(components[j]->GetStorableObjectFieldValue("entity_id").c_str()));
			assert(entity != 0);

			Component* component = (Component*)components[j];
			entity->AddComponent(component);
			component->SetActive(true);
			component->AddToSystem();
		}
	}

	// Set up server
	networkSystem->Listen(8053);

	// Set up replication system
	replicationSystem->SetType(REPLICATION_SERVER);

	// Create main loop timer
	Timer* mainTimer = new Timer();
	mainTimer->Start();
	float delta = 0.0f;
	float saveTimer = 0.0f;
	
	// Main loop
	while (true) {
		delta = mainTimer->Duration();
		saveTimer += delta;
		mainTimer->Reset();

		PROFILE_START_FRAME();

		if (saveTimer >= 5.0f) {
			std::vector<Entity*> entityList = entitySystem->GetEntities();
			std::vector<Entity*>::iterator i = entityList.begin();
			for (; i != entityList.end(); i++) {
				loader->SaveRecord(*i);

				std::vector<Component*> components = (*i)->GetComponents();
				for (size_t j = 0; j < components.size(); j++) {
					loader->SaveRecord(components[j]);
				}
			}

			saveTimer = 0.0f;
		}

		application->Update(delta);

		PROFILE_END_FRAME();
		Timer::Sleep(1);
	}

	// Dump profiler
	Profiler* profiler = Profiler::GetInstance();
	profiler->Log();

	application->Shutdown();
}
