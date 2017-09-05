
#include <giga-engine.h>

Application* Application::m_instance = 0;

Application::Application() {
    m_window = 0;
    m_outputLog = 0;
    m_loggingLevel = MSGTYPE_DEBUG;
}

Application::~Application() {
    for(size_t i = 0; i < m_systems.size(); i++) {
        delete m_systems[i];
    }
}

void Application::Initialize() {
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
     * Scripting interfaces
     */
    
    Application::Log(MSGTYPE_DEBUG, "Registering scripting types...");
    
    /**
     * Base
     */
    
    // Entity
    ScriptableObjectType* entityType = new ScriptableObjectType("Entity");
    entityType->AddObjectFunction("FindComponent", &Entity::FindComponent);
	entityType->AddObjectFunction("CreateComponent", &Entity::CreateComponent);
	entityType->AddObjectVariable("ID", &Entity::GetID, 0);
    
    scriptingSystem->RegisterScriptableObjectType<Entity>(entityType);
    
    // Entity System
    ScriptableObjectType* entitySystemType = new ScriptableObjectType("EntitySystem");
    entitySystemType->SetStatic(true);
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
    eventSystemType->SetStatic(true);
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
    inputSystemType->SetStatic(true);
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

	/**
	 * Networking
	 */

	// Network system
	ScriptableObjectType* networkSystemType = new ScriptableObjectType("NetworkSystem");
	networkSystemType->SetStatic(true);
	networkSystemType->AddStaticFunction("Send", &NetworkSystem::Send);

	scriptingSystem->RegisterScriptableObjectType<NetworkSystem>(networkSystemType);

	// Command messages
	ScriptableObjectType* commandMsgType = new ScriptableObjectType("CommandMessage");
	commandMsgType->SetTransient(true);
	
	scriptingSystem->RegisterScriptableObjectType<CommandMessage>(commandMsgType);

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
    resourceSystemType->SetStatic(true);
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
    
    Application::Log(MSGTYPE_DEBUG, "Registered scripting types...");

	/**
	 * Storable/serializable data types
	 */
	Application::Log(MSGTYPE_DEBUG, "Registering storable types...");

	StorableObjectType* meshStorableType = new StorableObjectType();
	meshStorableType->SetName("StaticMeshComponent");
	meshStorableType->SetPrimaryKey("mesh_id");
	meshStorableType->AddField("mesh", "Mesh", true, StorableObjectField::FIELD_RESOURCE);
	meshStorableType->AddField("position", "Position", true, StorableObjectField::FIELD_VECTOR3);
	meshStorableType->AddField("rotation", "Rotation", true, StorableObjectField::FIELD_QUATERNION);
	meshStorableType->AddField("scaling", "Scale", true, StorableObjectField::FIELD_VECTOR3);

	DataLoader::RegisterRecordType<StaticMeshComponent>(meshStorableType);

	StorableObjectType* scriptStorableType = new StorableObjectType();
	scriptStorableType->SetName("ScriptComponent");
	scriptStorableType->SetPrimaryKey("script_id");
	scriptStorableType->AddField("script", "Script", true, StorableObjectField::FIELD_RESOURCE);

	DataLoader::RegisterRecordType<ScriptComponent>(scriptStorableType);

	Application::Log(MSGTYPE_DEBUG, "Registered storable types...");
    
    Application::Log(MSGTYPE_INFO, "Initialization complete.");
}

void Application::Update(float delta) {
    for(size_t i = 0; i < m_systems.size(); i++) {
        m_systems[i]->Update(delta);
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
    
    for(size_t i = 0; i < m_systems.size(); i++) {
        m_systems[i]->Shutdown();
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

    application->m_outputLog->WriteLine(output);
    
    delete dt;
}

void Application::LogError(Error* error) {
    Application::Log(error->GetType(), error->GetMsg(), error->GetDetails());
}
