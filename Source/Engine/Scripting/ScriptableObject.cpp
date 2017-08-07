
#include <giga-engine.h>

ScriptableObject::ScriptableObject() {
    m_scriptType = 0;
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
    }
    
    v8::Local<v8::Value> handle = m_jsHandle.Get(isolate);
    v8::Local<v8::Object> obj = handle.As<v8::Object>();
    return(handle_scope.Escape(obj));
}
