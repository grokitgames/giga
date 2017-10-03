
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
    
    ScriptableObjectImpl* impl = thread->GetScriptableImpl(m_scriptType->GetName());
    
    // Then fill the handle if it is empty (otherwise just pass it in)
    if (m_jsHandle.IsEmpty()) {
        v8::Local<v8::Value> ret = impl->CreateJSObject();
        v8::Local<v8::Object> jsobj = ret.As<v8::Object>();
        Wrap(jsobj);
        m_jsHandle.Reset(isolate, jsobj);
		m_jsHandle.SetWeak<ScriptableObject>(this, __GCCallback, v8::WeakCallbackType::kParameter);
    }
    
    v8::Local<v8::Value> handle = m_jsHandle.Get(isolate);
    v8::Local<v8::Object> obj = handle.As<v8::Object>();
    return(handle_scope.Escape(obj));
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

	obj->m_jsHandle.Reset();
}
