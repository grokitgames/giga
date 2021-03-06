
#include <giga-engine.h>

ScriptThread::ScriptThread() {
	m_isolate = 0;
	m_currentScript = 0;
    m_locker = 0;
    m_currentLocker = 0;
}

ScriptThread::~ScriptThread() {

}

void ScriptThread::Initialize() {
	// Create an isolate
	v8::Isolate::CreateParams create_params;
	create_params.array_buffer_allocator = v8::ArrayBuffer::Allocator::NewDefaultAllocator();
	m_isolate = v8::Isolate::New(create_params);
    
    Lock(this);
    m_isolate->SetData(0, this);

	// Get all scriptable object type definitions and initialize in our isolate environment
	ScriptingSystem* scriptingSystem = GetSystem<ScriptingSystem>();

	std::vector<ScriptableObjectType*> types = scriptingSystem->GetRegisteredTypes();
	for (size_t i = 0; i < types.size(); i++) {
		ScriptableObjectImpl* impl = new ScriptableObjectImpl();
		impl->Create(types[i], this);

		m_impls.push_back(impl);
	}

    Unlock();
}

void ScriptThread::Shutdown() {
	if (m_isolate) {
		m_isolate->Dispose();
	}
}

void ScriptThread::Lock(ScriptThread* locker) {
    if(m_locker && locker == m_currentLocker) {
        return;
    }
    
    while(v8::Locker::IsLocked(m_isolate)) {
        Timer::Sleep(1);
    }
    
    GIGA_ASSERT(m_locker == 0, "Locker already exists.");
    
    m_locker = new v8::Locker(m_isolate);
    m_isolate->Enter();
    m_currentLocker = locker;
}

bool ScriptThread::IsLocked() {
    if(m_locker) {
        return(true);
    }
    
    return(false);
}

void ScriptThread::Unlock() {
    m_isolate->Exit();
    
    if(m_locker) {
        if(v8::Locker::IsLocked(m_isolate)) {
            delete m_locker;
        }
    }
    
    m_locker = 0;
    m_currentLocker = 0;
}

ScriptableObjectImpl* ScriptThread::GetScriptableImpl(std::string name) {
    // First, go through our implementations for this one
    for (size_t i = 0; i < m_impls.size(); i++) {
        if(m_impls[i]->GetName() == name) {
            return(m_impls[i]);
        }
    }
    
    // Then seeif it needs to be added
    ScriptingSystem* scriptingSystem = GetSystem<ScriptingSystem>();
    
    std::vector<ScriptableObjectType*> types = scriptingSystem->GetRegisteredTypes();
    ScriptableObjectImpl* impl = 0;
    for (size_t i = 0; i < types.size(); i++) {
        if(types[i]->GetName() == name) {
            impl = new ScriptableObjectImpl();
            impl->Create(types[i], this);
        
            m_impls.push_back(impl);
            break;
        }
    }
    
    return(impl);
}
