
#include <giga-engine.h>

ScriptThread::ScriptThread() {
	m_isolate = 0;
	m_currentScript = 0;
}

ScriptThread::~ScriptThread() {

}

void ScriptThread::Initialize() {
	// Create an isolate
	v8::Isolate::CreateParams create_params;
	create_params.array_buffer_allocator = v8::ArrayBuffer::Allocator::NewDefaultAllocator();
	m_isolate = v8::Isolate::New(create_params);
    
    m_isolate->SetData(0, this);

    Lock();

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

v8::Local<v8::Object> ScriptThread::GetCachedObject(ScriptableObject* obj) {
    v8::EscapableHandleScope scope(m_isolate);
    
    v8::Local<v8::Object> retval;
    std::map<ScriptableObject*, v8::Persistent<v8::Object, v8::CopyablePersistentTraits<v8::Object>>>::iterator it = m_vars.find(obj);
    if(it != m_vars.end()) {
		//printf("Returning stored pointer to object of type %s.\n", it->first->GetGigaName().c_str());
        retval = it->second.Get(m_isolate);
    }
    else {
		printf("Creating stored pointer to object of type %s.\n", obj->GetGigaName().c_str());

        obj->LockMutex();
        ScriptableObjectImpl* impl = this->GetScriptableImpl(obj->GetScriptType()->GetName());
        
        retval = impl->CreateJSObject();
        v8::Persistent<v8::Object, v8::CopyablePersistentTraits<v8::Value>> persistent;
        persistent.Reset(m_isolate, retval);
        obj->UnlockMutex();
        
        m_vars[obj] = persistent;
    }
    
    return(scope.Escape(retval));
}
