
#include <giga-engine.h>

ScriptThread::ScriptThread() {
	m_isolate = 0;
}

ScriptThread::~ScriptThread() {

}

void ScriptThread::Initialize() {
	// Create an isolate
	v8::Isolate::CreateParams create_params;
	create_params.array_buffer_allocator = v8::ArrayBuffer::Allocator::NewDefaultAllocator();
	m_isolate = v8::Isolate::New(create_params);

	m_isolate->Enter();

	// Get all scriptable object type definitions and initialize in our isolate environment
	ScriptingSystem* scriptingSystem = GetSystem<ScriptingSystem>();

	std::vector<ScriptableObjectType*> types = scriptingSystem->GetRegisteredTypes();
	for (size_t i = 0; i < types.size(); i++) {
		ScriptableObjectImpl* impl = new ScriptableObjectImpl();
		impl->Create(types[i]);

		m_types.push_back(impl);
	}

	m_isolate->Exit();
}

void ScriptThread::Shutdown() {
	if (m_isolate) {
		m_isolate->Dispose();
	}
}

void ScriptThread::Lock() {
    m_locker = new v8::Locker(m_isolate);
    m_isolate->Enter();
}

bool ScriptThread::IsLocked() {
    if(m_locker) {
        return(true);
    }
    
    return(false);
}

void ScriptThread::Unlock() {
    m_isolate->Exit();
    
    delete m_locker;
    m_locker = 0;
}

ScriptableObjectImpl* ScriptThread::GetScriptableImpl(std::string name) {
    // First, go through our implementations for this one
    for (size_t i = 0; i < m_types.size(); i++) {
        if(m_types[i]->GetName() == name) {
            return(m_types[i]);
        }
    }
    
    m_isolate->Enter();
    
    // Then seeif it needs to be added
    ScriptingSystem* scriptingSystem = GetSystem<ScriptingSystem>();
    
    std::vector<ScriptableObjectType*> types = scriptingSystem->GetRegisteredTypes();
    ScriptableObjectImpl* impl = 0;
    for (size_t i = 0; i < types.size(); i++) {
        ScriptableObjectImpl* impl = new ScriptableObjectImpl();
        impl->Create(types[i]);
        
        m_types.push_back(impl);
        break;
    }
    
    m_isolate->Exit();
    
    return(impl);
}
