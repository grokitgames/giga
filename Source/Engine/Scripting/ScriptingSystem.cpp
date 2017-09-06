
#include <giga-engine.h>

// Include "getcwd" function
#ifdef WIN32
#include <direct.h>
#else
#include <unistd.h>
#endif

ScriptingSystem::~ScriptingSystem() {
    // Delete stored global variables
    for(std::map<std::string, ScriptableVariant*>::iterator i = m_globals.begin(); i != m_globals.end(); i++) {
        delete i->second;
    }
    
    m_isolate->Dispose();
    v8::V8::ShutdownPlatform();
}

void ScriptingSystem::Initialize() {
    v8::V8::InitializeICU();
    v8::V8::Initialize();
    
    v8::Platform* platform = v8::platform::CreateDefaultPlatform();
    v8::V8::InitializePlatform(platform);
    
    char cwd[1000];
    getcwd(cwd, 1000);
    std::string file = cwd + std::string("/");
    
    v8::V8::InitializeExternalStartupData(file.c_str());
    
    v8::Isolate::CreateParams create_params;
    create_params.array_buffer_allocator = v8::ArrayBuffer::Allocator::NewDefaultAllocator();
    m_isolate = v8::Isolate::New(create_params);
    
    v8::Isolate::Scope isolate_scope(m_isolate);
    m_isolate->Enter();
}

void ScriptingSystem::RegisterGlobal(std::string name, Variant* value) {
    // Keep a copy of the variant for ourselves (so it doesn't get deleted)
    ScriptableVariant* variant = (ScriptableVariant*)(value);
    m_globals[name] = variant;
    
    // Add to any existing script components
    std::list<ScriptComponent*> scripts = m_scripts.GetList();
	std::list<ScriptComponent*>::iterator i = scripts.begin();
	for (i; i != scripts.end(); i++) {
        (*i)->SetGlobal(name, value);
    }
}

ScriptableObjectType* ScriptingSystem::GetScriptableObjectType(std::string name) {
    for(size_t i = 0; i < m_types.size(); i++) {
        if(m_types[i]->GetName() == name) {
            return(m_types[i]);
        }
    }
    
    return(0);
}

void ScriptingSystem::RegisterEventHandler(std::string type, std::string func, int entityID) {
	m_currentScript->RegisterEventHandler(type, func, entityID);
    
    // Register our callback for this type of event
    EventSystem* eventSystem = GetSystem<EventSystem>();
    eventSystem->RegisterEventHandler(type, ScriptingSystem::EventHandler);
}

void ScriptingSystem::EventHandler(Event* event) {
    // Locate our scripting system instance
    ScriptingSystem* scriptingSystem = GetSystem<ScriptingSystem>();
    
    // Iterate over event handlers
	std::list<ScriptComponent*> scripts = scriptingSystem->m_scripts.GetList();
	std::list<ScriptComponent*>::iterator i = scripts.begin();
	for (; i != scripts.end(); i++) {
		(*i)->ProcessEvent(event);
	}
}

void ScriptingSystem::Update(float delta) {
    PROFILE_START_AREA("ScriptingSystem Update");
	Variant* d = new Variant(delta);
    
	std::list<ScriptComponent*> scripts = m_scripts.GetList();
	std::list<ScriptComponent*>::iterator i = scripts.begin();

	for (i; i != scripts.end(); i++) {
		// Make sure this component is active
		if ((*i)->IsActive() == false) {
			continue;
		}

        Variant* parent = new Variant((*i)->GetParent());
		(*i)->SetGlobal("GameObject", parent);
		(*i)->CallFunction("Update", 1, &d);
        
        delete parent;
    }
    
    delete d;

	// Garbage collection
	v8::Isolate* isolate = v8::Isolate::GetCurrent();
	isolate->IdleNotification(1.0f / NETWORK_TICKS_PER_SECOND / 2.0f);

	// Remove any transient variables marked for deletion
	std::list<ScriptableObject*> transients = m_transients.GetList();
	std::list<ScriptableObject*>::iterator i2 = transients.begin();
	int counter = 0;
	for (i2; i2 != transients.end(); i2++) {
		m_transients.RemoveObject(*i2);
		delete (*i2);
		counter++;
	}

	if (counter) {
		printf("Garbage collected %d JS objects.\n", counter);
	}
    
    PROFILE_END_AREA("ScriptingSystem Update");
}

void ScriptingSystem::AddScriptComponent(Component* component) {
    ScriptingSystem* scriptingSystem = GetSystem<ScriptingSystem>();
	ScriptComponent* script = (ScriptComponent*)component;
    scriptingSystem->m_scripts.AddObject(script);
}

void ScriptingSystem::RemoveScriptComponent(Component* component) {
    ScriptingSystem* scriptingSystem = GetSystem<ScriptingSystem>();
    scriptingSystem->m_scripts.RemoveObject((ScriptComponent*)component);
}

void ScriptingSystem::AddTransient(ScriptableObject* object) {
	m_transients.AddObject(object);
}
