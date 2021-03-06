
#include <giga-engine.h>

// Include "getcwd" function
#ifdef WIN32
#include <direct.h>
#else
#include <unistd.h>
#endif

ScriptingSystem::ScriptingSystem() {
	m_platform = 0;
	m_taskPool = 0;
}

ScriptingSystem::~ScriptingSystem() {
    // Delete stored global variables
    for(std::map<std::string, ScriptableVariant*>::iterator i = m_globals.begin(); i != m_globals.end(); i++) {
        delete i->second;
    }

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

    ScriptThread::Initialize();
    
    // Initialize threads
    m_threadPool = new ThreadPool();
    for(int i = 0; i < 2; i++) {
        ScriptThread* thread = new ScriptThread();
        thread->Start(i + 1);
        
        m_threadPool->AddThread(thread);
    }
}

void ScriptingSystem::RegisterGlobal(std::string name, Variant* value) {
    // Keep a copy of the variant for ourselves (so it doesn't get deleted)
    ScriptableVariant* variant = (ScriptableVariant*)(value);
    m_globals[name] = variant;
    
    // Add to any existing script components
    std::vector<ScriptComponent*> scripts = m_scripts.GetList();
	std::vector<ScriptComponent*>::iterator i = scripts.begin();
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

void ScriptingSystem::RegisterEventHandler(ScriptComponent* component, std::string type, std::string func, int entityID) {
	component->RegisterEventHandler(type, func, entityID);
    
    // Register our callback for this type of event
    EventSystem* eventSystem = GetSystem<EventSystem>();
    eventSystem->RegisterEventHandler(type, ScriptingSystem::EventHandler);
}

void ScriptingSystem::EventHandler(Event* event) {
    // Locate our scripting system instance
    ScriptingSystem* scriptingSystem = GetSystem<ScriptingSystem>();
    
    // Iterate over event handlers
	std::vector<ScriptComponent*> scripts = scriptingSystem->m_scripts.GetList();
	std::vector<ScriptComponent*>::iterator i = scripts.begin();
	for (; i != scripts.end(); i++) {
		(*i)->ProcessEvent(event);
	}
}

void ScriptingSystem::Update(float delta) {
    PROFILE_START_AREA("ScriptingSystem Update");

	std::vector<ScriptComponent*> scripts = m_scripts.GetList();
	std::vector<ScriptComponent*>::iterator i = scripts.begin();

	if (m_taskPool == 0) {
		m_taskPool = new TaskPool();
	}

	for (i; i != scripts.end(); i++) {
		// Make sure this component is active
		if ((*i)->IsActive() == false) {
			continue;
		}

		Task* task = new Task();
		task->Create(*i, &ScriptComponent::Update);
		task->AddArgument(new Variant(delta));
		task->AddArgument(new Variant((*i)->GetParent()));

		m_taskPool->Push(task);
    }
    
	TaskSystem* taskSystem = GetSystem<TaskSystem>();
	taskSystem->Execute(m_taskPool, m_threadPool);

	/*for (i; i != scripts.end(); i++) {
		// Make sure this component is active
		if ((*i)->IsActive() == false) {
			continue;
		}

		Variant* parent = new Variant((*i)->GetParent());
		(*i)->SetGlobal("GameObject", parent);

		(*i)->CallFunction("Update", 1, &d);
	}*/

	// Remove any transient variables marked for deletion
	std::vector<ScriptableObject*> transients = m_transients.GetList();
	std::vector<ScriptableObject*>::iterator i2 = transients.begin();
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
