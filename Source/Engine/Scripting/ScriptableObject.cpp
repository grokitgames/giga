
#include <giga-engine.h>

ScriptableObject::ScriptableObject() {
    m_scriptType = 0;
	m_deleted = false;
}

v8::Local<v8::Object> ScriptableObject::GetJSObject() {
    v8::Isolate* isolate = v8::Isolate::GetCurrent();
    v8::EscapableHandleScope handle_scope(isolate);
    
    // Lazy load interface
    if (m_scriptType == 0) {
        ScriptingSystem* scriptingSystem = GetSystem<ScriptingSystem>();
        m_scriptType = scriptingSystem->GetScriptableObjectType(this->GetGigaName());
        
        GIGA_ASSERT(m_scriptType != 0, "Interface not defined for class type.")
    }
    
    // Then fill the handle if it is empty (otherwise just pass it in)
    if (m_jsHandle.IsEmpty()) {
        v8::Local<v8::Value> ret = m_scriptType->CreateJSObject();
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
	obj->m_deleted = true; // TODO: Need to find a way to delete these objects after we're done with them
	obj->m_jsHandle.Reset();
}