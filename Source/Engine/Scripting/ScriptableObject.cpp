
#include <giga-engine.h>

ScriptableObject::ScriptableObject() {
    m_scriptType = 0;
	m_deleted = false;
}

ScriptableObject::~ScriptableObject() {
    std::map<ScriptThread*, v8::Persistent<v8::Object, v8::CopyablePersistentTraits<v8::Object>>>::iterator it = m_cached.begin();
    for(; it != m_cached.end(); it++) {
        it->second.Reset();
    }
}

v8::Local<v8::Object> ScriptableObject::GetJSObject() {
    v8::Isolate* isolate = v8::Isolate::GetCurrent();
    v8::EscapableHandleScope handle_scope(isolate);
    
    // Get the thread we are currently executing on
    ScriptThread* thread = (ScriptThread*)isolate->GetData(0);

	v8::Local<v8::Object> retval;
	std::map<ScriptThread*, v8::Persistent<v8::Object, v8::CopyablePersistentTraits<v8::Object>>>::iterator it = m_cached.find(thread);
	if (it != m_cached.end()) {
		retval = it->second.Get(isolate);
		return(handle_scope.Escape(retval));
	}
    
	LockMutex();

    // Lazy load interface
    if (m_scriptType == 0) {
        ScriptingSystem* scriptingSystem = GetSystem<ScriptingSystem>();
        m_scriptType = scriptingSystem->GetScriptableObjectType(this->GetGigaName());
        
        GIGA_ASSERT(m_scriptType != 0, "Interface not defined for class type.")
    }
    
	ScriptableObjectImpl* impl = thread->GetScriptableImpl(GetScriptType()->GetName());

	retval = impl->CreateJSObject();
	v8::Persistent<v8::Object, v8::CopyablePersistentTraits<v8::Value>> persistent;
	persistent.Reset(isolate, retval);

    Wrap(retval);
	m_cached[thread] = persistent;

	UnlockMutex();

    return(handle_scope.Escape(retval));
}

void ScriptableObject::__GCCallback(const v8::WeakCallbackInfo<ScriptableObject>& data) {
	ScriptableObject* obj = data.GetParameter();
	
	// Mark deleted and add to transient queue for removal
	obj->m_deleted = true;
	if (obj->m_scriptType == 0) {
		ScriptingSystem* scriptingSystem = GetSystem<ScriptingSystem>();
		obj->m_scriptType = scriptingSystem->GetScriptableObjectType(obj->GetGigaName());
	}

	if (obj->m_scriptType->IsTransient()) {
		ScriptingSystem* scriptingSystem = GetSystem<ScriptingSystem>();
		scriptingSystem->AddTransient(obj);
	}

	//obj->m_jsHandle.Reset();
}
