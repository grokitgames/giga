
#include <giga-engine.h>

ScriptableObject::ScriptableObject() {
    m_scriptType = 0;
	m_deleted = false;
}

v8::Local<v8::Object> ScriptableObject::GetJSObject() {
    v8::Isolate* isolate = v8::Isolate::GetCurrent();
    v8::EscapableHandleScope handle_scope(isolate);
    
    // Get the thread we are currently executing on
    ScriptThread* thread = (ScriptThread*)isolate->GetData(0);
    
    // Lazy load interface
    if (m_scriptType == 0) {
        ScriptingSystem* scriptingSystem = GetSystem<ScriptingSystem>();
        m_scriptType = scriptingSystem->GetScriptableObjectType(this->GetGigaName());
        
        GIGA_ASSERT(m_scriptType != 0, "Interface not defined for class type.")
    }
    
    // Check if we have handle for this variable already
    v8::Local<v8::Object> ret = thread->GetCachedObject(this);
    if(ret->IsNull() == true) {
        GIGA_ASSERT(false, "Cannot get JS object.");
    }
    
    Wrap(ret);
    return(handle_scope.Escape(ret));
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
