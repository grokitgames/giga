
#include <giga-engine.h>

Application* Application::m_instance = 0;

Application::Application() {
    m_window = 0;
    m_outputLog = 0;
    m_loggingLevel = 0;
}

Application::~Application() {
    std::vector<RegisteredSystem*>::iterator i = m_systems.begin();
    for(; i != m_systems.end(); i++) {
        delete (*i)->system;
        delete (*i);
    }
    
    m_systems.clear();
}

void Application::Initialize() {
	/**
	 * Initialize all systems
	 */
	std::vector<RegisteredSystem*>::iterator i = m_systems.begin();
	for (; i != m_systems.end(); i++) {
		System* sys = (*i)->system;
		sys->Initialize();
		sys->InitializeThread(0);
	}

    /**
     * Output log
     */
    
    // Open log file
    m_outputLog = new File("output.txt", FILEMODE_APPEND);
    
    // Log startup time
    m_outputLog->WriteLine("\n------------------------------\n");
    Application::Log(MSGTYPE_INFO, "Initializing application...");
    
    /**
     * Get core systems
     */
    ScriptingSystem* scriptingSystem = GetSystem<ScriptingSystem>();
    GIGA_ASSERT(scriptingSystem != 0, "Required scripting system is not registered.");
    
    ErrorSystem* errorSystem = GetSystem<ErrorSystem>();
    GIGA_ASSERT(errorSystem != 0, "Required error system is not registered.");
    
    ResourceSystem* resourceSystem = GetSystem<ResourceSystem>();
    GIGA_ASSERT(resourceSystem != 0, "Required resource system is not registered.");
    
    NetworkSystem* networkSystem = GetSystem<NetworkSystem>();

    /**
     * Error handling
     */
    
    // Register for errors to log
    errorSystem->RegisterErrorHandler(ERROR_INFO, &Application::LogError);
    errorSystem->RegisterErrorHandler(ERROR_DEBUG, &Application::LogError);
    errorSystem->RegisterErrorHandler(ERROR_WARN, &Application::LogError);
    errorSystem->RegisterErrorHandler(ERROR_FATAL, &Application::LogError);
    
    /**
     * Register resource types
     */
    
    Application::Log(MSGTYPE_DEBUG, "Registering resource types...");
    
    resourceSystem->RegisterResourceType<Script>("Script");
    resourceSystem->RegisterResourceType<Mesh>("Mesh");
    resourceSystem->RegisterResourceType<Shader>("Shader");
	resourceSystem->RegisterResourceType<Texture2D>("Texture2D");
	    
    Application::Log(MSGTYPE_DEBUG, "Registered resource types...");


	/**
	* Networking message types
	*/

	Application::Log(MSGTYPE_DEBUG, "Registering network message types...");

	networkSystem->RegisterMessageType<EchoRequestMessage>(10);
	networkSystem->RegisterMessageType<EchoResponseMessage>(20);
	networkSystem->RegisterMessageType<EntitySnapshotMessage>(30);
	networkSystem->RegisterMessageType<CommandMessage>(40);
	networkSystem->RegisterMessageType<ResendRequestMessage>(50);
	networkSystem->RegisterMessageType<AckMessage>(60);

	Application::Log(MSGTYPE_DEBUG, "Registered network message types...");
    
    /**
     * Register component types
     */
    
    Application::Log(MSGTYPE_DEBUG, "Registering component types...");
    
    Component::RegisterComponentType<ScriptComponent>("ScriptComponent", 10, &ScriptingSystem::AddScriptComponent, &ScriptingSystem::RemoveScriptComponent);
    Component::RegisterComponentType<StaticMeshComponent>("StaticMeshComponent", 20, &RenderSystem::AddStaticMeshComponent, &RenderSystem::RemoveStaticMeshComponent);
    Component::RegisterComponentType<CameraComponent>("CameraComponent", 30, &RenderSystem::AddCameraComponent, &RenderSystem::RemoveCameraComponent);
    Component::RegisterComponentType<SpotLightComponent>("SpotLightComponent", 40, &RenderSystem::AddSpotLightComponent, &RenderSystem::RemoveSpotLightComponent);
    
    Application::Log(MSGTYPE_DEBUG, "Registered component types...");

	/**
	 * Storable/serializable data types
	 */
	Application::Log(MSGTYPE_DEBUG, "Registering storable types...");

	StorableObjectType* entityStorableType = new StorableObjectType();
	entityStorableType->SetName("Entity");
	entityStorableType->SetPrimaryKey("entity_id");
	entityStorableType->AddField("scene_id", StorableObjectField::FIELD_FOREIGNKEY);
	entityStorableType->AddField("entity_name", StorableObjectField::FIELD_TEXT);
	//entityStorableType->AddField("status", StorableObjectField::FIELD_INT);

	DataLoader::RegisterRecordType<Entity>(entityStorableType);

	StorableObjectType* meshStorableType = new StorableObjectType();
	meshStorableType->SetName("StaticMeshComponent");
	meshStorableType->SetPrimaryKey("mesh_id");
	meshStorableType->AddField("scene_id", StorableObjectField::FIELD_FOREIGNKEY);
	meshStorableType->AddField("entity_id", StorableObjectField::FIELD_FOREIGNKEY);
	meshStorableType->AddField("mesh", "Mesh", true, StorableObjectField::FIELD_RESOURCE);
	meshStorableType->AddField("position", "Position", true, StorableObjectField::FIELD_VECTOR3);
	meshStorableType->AddField("rotation", "Rotation", true, StorableObjectField::FIELD_QUATERNION);
	meshStorableType->AddField("scaling", "Scale", true, StorableObjectField::FIELD_VECTOR3);

	DataLoader::RegisterRecordType<StaticMeshComponent>(meshStorableType);

	StorableObjectType* scriptStorableType = new StorableObjectType();
	scriptStorableType->SetName("ScriptComponent");
	scriptStorableType->SetPrimaryKey("script_id");
	scriptStorableType->AddField("scene_id", StorableObjectField::FIELD_FOREIGNKEY);
	scriptStorableType->AddField("entity_id", StorableObjectField::FIELD_FOREIGNKEY);
	scriptStorableType->AddField("script", "Script", true, StorableObjectField::FIELD_RESOURCE);

	DataLoader::RegisterRecordType<ScriptComponent>(scriptStorableType);
    
    StorableObjectType* spotLightStorableType = new StorableObjectType();
    spotLightStorableType->SetName("SpotLightComponent");
    spotLightStorableType->SetPrimaryKey("spotlight_id");
    spotLightStorableType->AddField("scene_id", StorableObjectField::FIELD_FOREIGNKEY);
    spotLightStorableType->AddField("entity_id", StorableObjectField::FIELD_FOREIGNKEY);
    spotLightStorableType->AddField("position", "Position", true, StorableObjectField::FIELD_VECTOR3);
    spotLightStorableType->AddField("color", "Color", true, StorableObjectField::FIELD_VECTOR3);
    spotLightStorableType->AddField("attenuation", "Attenuation", true, StorableObjectField::FIELD_FLOAT);
    
    DataLoader::RegisterRecordType<SpotLightComponent>(spotLightStorableType);

	Application::Log(MSGTYPE_DEBUG, "Registered storable types...");

	InitializeThread(0);
    
    Application::Log(MSGTYPE_INFO, "Initialization complete.");
}

void Application::InitializeThread(int threadID) {
	/**
	* Scripting interfaces
	*/

	Application::Log(MSGTYPE_DEBUG, "Registering scripting types...");

	ScriptingSystem* scriptingSystem = GetSystem<ScriptingSystem>();

	/**
	* Base
	*/
    
    scriptingSystem->Lock(scriptingSystem);

	// Application
	ScriptableObjectType* applicationType = new ScriptableObjectType("Application");
	applicationType->SetStatic(true, this);
	applicationType->AddStaticFunction("Log", &Application::Log);

	scriptingSystem->RegisterScriptableObjectType<Application>(applicationType);

	// Entity
	ScriptableObjectType* entityType = new ScriptableObjectType("Entity");
	entityType->AddObjectFunction("FindComponent", &Entity::FindComponent);
	entityType->AddObjectFunction("CreateComponent", &Entity::CreateComponent);
	entityType->AddObjectVariable("ID", &Entity::GetID, 0);

	scriptingSystem->RegisterScriptableObjectType<Entity>(entityType);

	// Entity System
	ScriptableObjectType* entitySystemType = new ScriptableObjectType("EntitySystem");
	entitySystemType->SetStatic(true, GetSystem<EntitySystem>());
	entitySystemType->AddStaticFunction("FindEntity", &EntitySystem::FindEntity);

	scriptingSystem->RegisterScriptableObjectType<EntitySystem>(entitySystemType);

	// Events
	ScriptableObjectType* eventType = new ScriptableObjectType("Event");
	eventType->AddObjectVariable("type", &Event::GetType, 0);
	eventType->AddObjectVariable("data", &Event::GetData, 0);
	eventType->AddObjectVariable("entityID", &Event::GetEntityID, 0);

	scriptingSystem->RegisterScriptableObjectType<Event>(eventType);

	// Event System
	ScriptableObjectType* eventSystemType = new ScriptableObjectType("EventSystem");
	eventSystemType->SetStatic(true, GetSystem<EventSystem>());
	eventSystemType->AddStaticFunction("Process", &EventSystem::Process);
	eventSystemType->AddStaticFunction("RegisterEventHandler", &EventSystem::RegisterEventHandler);

	scriptingSystem->RegisterScriptableObjectType<EventSystem>(eventSystemType);

	// Component template type
	ScriptableObjectType* componentTemplateType = new ScriptableObjectType("ComponentTemplateType");
	componentTemplateType->AddObjectFunction("CreateField", &ComponentTemplateType::CreateField);
	componentTemplateType->AddObjectFunction("SetName", &ComponentTemplateType::SetName);
	componentTemplateType->AddObjectFunction("Register", &ComponentTemplateType::Register);

	scriptingSystem->RegisterScriptableObjectType<ComponentTemplateType>(componentTemplateType);

	// Component field types
	scriptingSystem->RegisterGlobal("FIELD_INT", new Variant(StorableObjectField::FIELD_INT));
	scriptingSystem->RegisterGlobal("FIELD_RESOURCE", new Variant(StorableObjectField::FIELD_RESOURCE));
	scriptingSystem->RegisterGlobal("FIELD_TEXT", new Variant(StorableObjectField::FIELD_TEXT));
	scriptingSystem->RegisterGlobal("FIELD_FLOAT", new Variant(StorableObjectField::FIELD_FLOAT));
	scriptingSystem->RegisterGlobal("FIELD_VECTOR2", new Variant(StorableObjectField::FIELD_VECTOR2));
	scriptingSystem->RegisterGlobal("FIELD_VECTOR3", new Variant(StorableObjectField::FIELD_VECTOR3));
	scriptingSystem->RegisterGlobal("FIELD_QUATERNION", new Variant(StorableObjectField::FIELD_QUATERNION));

	/**
	* I/O
	*/

	// Input system
	ScriptableObjectType* inputSystemType = new ScriptableObjectType("InputSystem");
	inputSystemType->SetStatic(true, GetSystem<InputSystem>());
	inputSystemType->AddStaticFunction("GetInputDevice", &InputSystem::GetInputDevice);
	inputSystemType->AddStaticFunction("RegisterInputMapping", &InputSystem::RegisterInputMapping);
	inputSystemType->AddStaticFunction("FindInputMapping", &InputSystem::FindInputMapping);

	scriptingSystem->RegisterScriptableObjectType<InputSystem>(inputSystemType);

	// Device types
	scriptingSystem->RegisterGlobal("INPUTDEVICE_KEYBOARD", new Variant(INPUTDEVICE_KEYBOARD));
	scriptingSystem->RegisterGlobal("INPUTDEVICE_MOUSE", new Variant(INPUTDEVICE_MOUSE));
	scriptingSystem->RegisterGlobal("INPUTDEVICE_JOYSTICK", new Variant(INPUTDEVICE_JOYSTICK));

	// Commands
	ScriptableObjectType* commandType = new ScriptableObjectType("Command");
	commandType->AddStaticFunction("RegisterCommandType", &Command::RegisterCommandType);
	commandType->AddObjectFunction("Start", &Command::Start);
	commandType->AddObjectFunction("End", &Command::End);
	commandType->AddObjectVariable("type", &Command::Type, 0);

	scriptingSystem->RegisterScriptableObjectType<Command>(commandType);

	// Command types
	scriptingSystem->RegisterGlobal("COMMAND_START", new Variant(COMMAND_START));
	scriptingSystem->RegisterGlobal("COMMAND_END", new Variant(COMMAND_END));

	// Files
	ScriptableObjectType* fileType = new ScriptableObjectType("File");
	fileType->AddStaticFunction("Load", &File::Load);

	scriptingSystem->RegisterScriptableObjectType<File>(fileType);

	/**
	* Networking
	*/

	// Network system
	ScriptableObjectType* networkSystemType = new ScriptableObjectType("NetworkSystem");
	networkSystemType->SetStatic(true, GetSystem<NetworkSystem>());
	networkSystemType->AddStaticFunction("Connect", &NetworkSystem::Connect);
	networkSystemType->AddStaticFunction("Send", &NetworkSystem::Send);
	networkSystemType->AddStaticFunction("SetSessionID", &NetworkSystem::SetSessionID);
	networkSystemType->AddStaticFunction("SetPlayerID", &NetworkSystem::SetPlayerID);

	scriptingSystem->RegisterScriptableObjectType<NetworkSystem>(networkSystemType);

	// Replication system
	ScriptableObjectType* replicationSystemType = new ScriptableObjectType("ReplicationSystem");
	replicationSystemType->SetStatic(true, GetSystem<ReplicationSystem>());

	scriptingSystem->RegisterScriptableObjectType<ReplicationSystem>(replicationSystemType);

	// HttpRequest
	ScriptableObjectType* httpRequestType = new ScriptableObjectType("HttpRequest");
	httpRequestType->AddObjectFunction("Execute", &HttpRequest::Execute);
	httpRequestType->AddObjectFunction("AddPostData", &HttpRequest::AddPostData);

	scriptingSystem->RegisterScriptableObjectType<HttpRequest>(httpRequestType);

	/**
	* Platform
	*/

	// DateTime
	ScriptableObjectType* dateTimeType = new ScriptableObjectType("DateTime");
	dateTimeType->AddStaticFunction("GetCurrent", &DateTime::GetCurrent);
	dateTimeType->AddObjectVariable("year", &DateTime::GetYear, 0);
	dateTimeType->AddObjectVariable("month", &DateTime::GetMonth, 0);
	dateTimeType->AddObjectVariable("day", &DateTime::GetDay, 0);
	dateTimeType->AddObjectVariable("hour", &DateTime::GetHour, 0);
	dateTimeType->AddObjectVariable("minute", &DateTime::GetMinute, 0);
	dateTimeType->AddObjectVariable("second", &DateTime::GetSecond, 0);
	dateTimeType->AddObjectVariable("timestamp", &DateTime::GetTimestamp, 0);

	scriptingSystem->RegisterScriptableObjectType<DateTime>(dateTimeType);

	/**
	* Resources
	*/

	// Resource system
	ScriptableObjectType* resourceSystemType = new ScriptableObjectType("ResourceSystem");
	resourceSystemType->SetStatic(true, GetSystem<ResourceSystem>());
	resourceSystemType->AddStaticFunction("LoadResource", &ResourceSystem::LoadResource);

	scriptingSystem->RegisterScriptableObjectType<ResourceSystem>(resourceSystemType);

	/**
	* Rendering
	*/

	// Transform
	ScriptableObjectType* transformType = new ScriptableObjectType("Transform");
	transformType->AddObjectVariable("position", &Transform::GetPosition, &Transform::SetPosition);
	transformType->AddObjectVariable("rotation", &Transform::GetRotation, &Transform::SetRotation);
	transformType->AddObjectVariable("scaling", &Transform::GetScaling, &Transform::SetScaling);
	transformType->AddObjectVariable("look", &Transform::GetLook, 0);
	transformType->AddObjectVariable("up", &Transform::GetUp, 0);
	transformType->AddObjectVariable("right", &Transform::GetRight, 0);
	transformType->AddObjectFunction("Translate", &Transform::Translate);
	transformType->AddObjectFunction("Rotate", &Transform::Rotate);
	transformType->AddObjectFunction("Scale", &Transform::Scale);

	scriptingSystem->RegisterScriptableObjectType<Transform>(transformType);

	// Vector class
	ScriptableObjectType* vector3Type = new ScriptableObjectType("Vector3");
	vector3Type->AddObjectVariable("x", &Vector3::GetX, &Vector3::SetX);
	vector3Type->AddObjectVariable("y", &Vector3::GetY, &Vector3::SetY);
	vector3Type->AddObjectVariable("z", &Vector3::GetZ, &Vector3::SetZ);
	vector3Type->SetTransient(true);

	scriptingSystem->RegisterScriptableObjectType<Vector3>(vector3Type);

	// Quaternions
	ScriptableObjectType* quaternionType = new ScriptableObjectType("Quaternion");
	quaternionType->AddObjectVariable("x", &Quaternion::GetX, &Quaternion::SetX);
	quaternionType->AddObjectVariable("y", &Quaternion::GetY, &Quaternion::SetY);
	quaternionType->AddObjectVariable("z", &Quaternion::GetZ, &Quaternion::SetZ);
	quaternionType->AddObjectVariable("w", &Quaternion::GetW, &Quaternion::SetW);
	quaternionType->SetTransient(true);

	scriptingSystem->RegisterScriptableObjectType<Quaternion>(quaternionType);

	// Transform component
	ScriptableObjectType* transformComponentType = new ScriptableObjectType("TransformComponent");
	transformComponentType->AddObjectVariable("transform", &TransformComponent::GetTransform, &TransformComponent::SetTransform);

	scriptingSystem->RegisterScriptableObjectType<TransformComponent>(transformComponentType);

	// Static mesh component
	ScriptableObjectType* staticMeshType = new ScriptableObjectType("StaticMeshComponent");
	staticMeshType->AddObjectVariable("transform", &StaticMeshComponent::GetTransform, &StaticMeshComponent::SetTransform);
	staticMeshType->AddObjectFunction("Instantiate", &StaticMeshComponent::Instantiate);

	scriptingSystem->RegisterScriptableObjectType<StaticMeshComponent>(staticMeshType);

	// Camera component
	ScriptableObjectType* cameraType = new ScriptableObjectType("CameraComponent");
	cameraType->AddObjectVariable("transform", &CameraComponent::GetTransform, &CameraComponent::SetTransform);
	cameraType->AddObjectFunction("SetViewport", &CameraComponent::SetViewport);
	cameraType->AddObjectVariable("fov", &CameraComponent::GetFOV, &CameraComponent::SetFOV);
	cameraType->AddObjectVariable("near", &CameraComponent::GetNear, &CameraComponent::SetNear);
	cameraType->AddObjectVariable("far", &CameraComponent::GetFar, &CameraComponent::SetFar);
	cameraType->AddObjectVariable("look", &CameraComponent::GetLook, &CameraComponent::SetLook);
	cameraType->AddObjectVariable("up", &CameraComponent::GetUp, 0);
	cameraType->AddObjectVariable("right", &CameraComponent::GetRight, 0);

	scriptingSystem->RegisterScriptableObjectType<CameraComponent>(cameraType);

	// Render system
	ScriptableObjectType* renderSystemType = new ScriptableObjectType("RenderSystem");
	renderSystemType->SetStatic(true, this);
	renderSystemType->AddStaticFunction("GetWindowWidth", &RenderSystem::GetWindowWidth);
	renderSystemType->AddStaticFunction("GetWindowHeight", &RenderSystem::GetWindowHeight);
	renderSystemType->AddStaticFunction("SetActiveCamera", &RenderSystem::SetActiveCamera);

	scriptingSystem->RegisterScriptableObjectType<RenderSystem>(renderSystemType);
    
    scriptingSystem->Unlock();

	Application::Log(MSGTYPE_DEBUG, "Registered scripting types...");
}

void Application::Update(float delta) {
    std::vector<RegisteredSystem*>::iterator i = m_systems.begin();

    for(; i != m_systems.end(); i++) {
        if((*i)->tickRate > 0) {
            (*i)->accumulator += delta;
            
            if((*i)->accumulator > (1.0f / (*i)->tickRate)) {
                float theta = (1.0f / (*i)->tickRate);
                (*i)->accumulator -= theta;
                (*i)->system->Update(theta);
            }
        }
    }
    
    i = m_systems.begin();
    for(; i != m_systems.end(); i++) {
        if((*i)->tickRate == 0) {
            (*i)->system->Update(delta);
        }
    }
}

Application* Application::GetInstance() {
    if(m_instance == 0) {
        m_instance = new Application();
    }
    
    return(m_instance);
}

void Application::Shutdown() {
    Application::Log(MSGTYPE_INFO, "Shutting down...");
    
    std::vector<RegisteredSystem*>::iterator i = m_systems.begin();
    for(; i != m_systems.end(); i++) {
        (*i)->system->Shutdown();
    }
    
    Application::Log(MSGTYPE_INFO, "Shut down complete.");
}

void Application::Log(int type, std::string message, std::string details) {
    std::string output;
    Application* application = Application::GetInstance();
    
    if(type < application->m_loggingLevel) {
        return;
    }
    
    // Prepend date/time
    DateTime* dt = DateTime::GetCurrent();
    output += "[" + dt->ToString() + "] ";
    
    // Prepend some info
    switch(type) {
        case MSGTYPE_INFO:
            output += "[INFO ]: ";
            break;
        case MSGTYPE_DEBUG:
            output += "[DEBUG]: ";
            break;
        case MSGTYPE_WARN:
            output += "[WARN ]: ";
            break;
        case MSGTYPE_FATAL:
            output += "[FATAL]: ";
            break;
        default:
            output += "[INFO ]: ";
    }
    
    // Append the message
    output += message;
    
    // Then any details
    if(details.length()) {
        output += " (" + details + ")";
    }
    
	printf("%s\n", output.c_str());

#ifdef WIN32
	OutputDebugString(output.c_str());
#endif
	
    application->m_outputLog->WriteLine(output);
    
    delete dt;
}

void Application::LogError(Error* error) {
    Application::Log(error->GetType(), error->GetMsg(), error->GetDetails());
}

std::vector<System*> Application::GetSystems() {
	std::vector<System*> systems;
	for (size_t i = 0; i < m_systems.size(); i++) {
		systems.push_back(m_systems[i]->system);
	}

	return(systems);
}

Variant* Application::Log(Variant* object, int argc, Variant** argv) {
	GIGA_ASSERT(argc == 1, "Log expects one argument.");
	GIGA_ASSERT(argv[0]->IsString(), "First parameter should be string log line output.");

	Application* application = Application::GetInstance();
	application->Log(MSGTYPE_DEBUG, argv[0]->AsString());

	return(new Variant(0));
}
