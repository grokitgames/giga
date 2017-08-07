
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
    
    // Delete event handlers
    for(size_t i = 0; i < m_eventHandlers.size(); i++) {
        delete m_eventHandlers[i];
    }
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
    std::vector<ScriptComponent*> scripts = m_scripts.GetList();
    for(size_t i = 0; i < scripts.size(); i++) {
        scripts[i]->SetGlobal(name, value);
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

void ScriptingSystem::RegisterEventHandler(std::string type, std::string func, GigaObject* sender) {
    // Create a new event handler
    struct EventHandler* handler = new struct EventHandler();
    handler->type = type;
    handler->sender = sender;
    handler->func = func;
    handler->script = m_currentScript;
    
    m_eventHandlers.push_back(handler);
    
    // Register our callback for this type of event
    EventSystem* eventSystem = GetSystem<EventSystem>();
    eventSystem->RegisterEventHandler(type, ScriptingSystem::EventHandler);
}

void ScriptingSystem::EventHandler(Event* event) {
    // Locate our scripting system instance
    ScriptingSystem* scriptingSystem = GetSystem<ScriptingSystem>();
    
    // Iterate over event handlers
    std::string type = event->GetType();
    for(size_t i = 0; i < scriptingSystem->m_eventHandlers.size(); i++) {
        if(scriptingSystem->m_eventHandlers[i]->type == type) {
            if(scriptingSystem->m_eventHandlers[i]->sender == 0 || scriptingSystem->m_eventHandlers[i]->sender == event->GetSender()) {
                Variant* v = new Variant(event);
                scriptingSystem->m_eventHandlers[i]->script->CallFunction(scriptingSystem->m_eventHandlers[i]->func, 1, &v);
                delete v;
            }
        }
    }
}

void ScriptingSystem::Update(float delta) {
    std::vector<ScriptComponent*> scripts = m_scripts.GetList();
    Variant* d = new Variant(delta);
    for(size_t i = 0; i < scripts.size(); i++) {
        scripts[i]->SetGlobal("GameObject", new Variant(scripts[i]->GetParent()));
        scripts[i]->CallFunction("Update", 1, &d);
    }
    
    delete d;
}

Component* ScriptingSystem::CreateScriptComponent() {
    ScriptingSystem* scriptingSystem = GetSystem<ScriptingSystem>();
    
    ScriptComponent* script = dynamic_cast<ScriptComponent*>(Component::CreateComponent("ScriptComponent"));
    scriptingSystem->m_scripts.AddObject(script);
    return(script);
}

void ScriptingSystem::RemoveScriptComponent(Component* component) {
    ScriptingSystem* scriptingSystem = GetSystem<ScriptingSystem>();
    
    scriptingSystem->m_scripts.RemoveObject(dynamic_cast<ScriptComponent*>(component));
}
