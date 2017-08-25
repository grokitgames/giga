
#include <giga-engine.h>

ScriptableObjectType::ScriptableObjectType(std::string name) {
    m_static = false;
    
    SetName(name);
    StartTemplate();
}

void ScriptableObjectType::StartTemplate() {
    // Start template
    v8::Isolate* isolate = v8::Isolate::GetCurrent();
    v8::HandleScope handle_scope(isolate);
    
    v8::Local<v8::FunctionTemplate> tpl = v8::FunctionTemplate::New(isolate, New);
    m_functionTemplate.Reset(isolate, tpl);
}

void ScriptableObjectType::EndTemplate() {
    // Set our internal field count to 1 to we can house our "this" object for reference later
    v8::Isolate* isolate = v8::Isolate::GetCurrent();
    v8::HandleScope handle_scope(isolate);
    
    v8::Local<v8::FunctionTemplate> tpl = m_functionTemplate.Get(isolate);
    tpl->InstanceTemplate()->SetInternalFieldCount(1);
    
    // Set our class name
    tpl->SetClassName(v8::String::NewFromUtf8(isolate, m_typeName.c_str()));
}

void ScriptableObjectType::New(const v8::FunctionCallbackInfo<v8::Value>& info) {
    v8::HandleScope scope(info.GetIsolate());
    
    // Convert our callback info into SmartValue objects
    Variant** args = (Variant**)malloc(sizeof(Variant*) * info.Length());
    for (int i = 0; i < info.Length(); i++) {
        args[i] = new ScriptableVariant(info[i]);
    }
    
    v8::Local<v8::Function> func = info.Callee();
    v8::String::Utf8Value funcName(func->GetName());
    
    // Get the script interface we're dealing with
    ScriptingSystem* ss = GetSystem<ScriptingSystem>();
    ScriptableObjectType* interface = ss->GetScriptableObjectType(*funcName);
    
    ScriptableObject* obj = interface->m_createFunction();
    obj->InitializeScriptableObject(interface->GetName());
    obj->Initialize(args, info.Length());
    if (interface->m_static == false) {
        obj->Wrap(info.This());
    }
    else {
        GIGA_ASSERT(false, "The new operator is not supported on static global variables.");
    }
    
    for (int i = 0; i < info.Length(); i++) {
        delete args[i];
    }
    free(args);
    
    return info.GetReturnValue().Set(info.This());
}

void ScriptableObjectType::AddObjectVariable(std::string name, ScriptObjectGetterFunc getter, ScriptObjectSetterFunc setter) {
    v8::Isolate* isolate = v8::Isolate::GetCurrent();
    v8::HandleScope handle_scope(isolate);
    
    // Create a new variable function callback
    ScriptObjectCallbackPair* pair = new ScriptObjectCallbackPair();
    
    // Copy params
    pair->variableName = name;
    pair->getter = getter;
    pair->setter = setter;
    
    // Set this up as an accessible variable name
    v8::Local<v8::FunctionTemplate> tpl = m_functionTemplate.Get(isolate);
    tpl->InstanceTemplate()->SetAccessor(v8::String::NewFromUtf8(isolate, name.c_str()), HandleObjectGetter, HandleObjectSetter);
    
    m_varList.push_back(pair);
}

void ScriptableObjectType::AddObjectFunction(std::string name, ScriptObjectFunc func) {
    v8::Isolate* isolate = v8::Isolate::GetCurrent();
    v8::HandleScope handle_scope(isolate);
    
    // Create a new variable function callback
    ScriptFunctionCallback* pair = new ScriptFunctionCallback();
    
    // Copy params
    pair->variableName = name;
    pair->func = func;
    
    // Set this up as an accessible variable name
    v8::Local<v8::FunctionTemplate> tpl = m_functionTemplate.Get(isolate);
    tpl->InstanceTemplate()->Set(v8::String::NewFromUtf8(isolate, name.c_str()), v8::FunctionTemplate::New(isolate, HandleObjectFunctionCallback));
    
    m_funcList.push_back(pair);
}

void ScriptableObjectType::AddStaticVariable(std::string name, ScriptObjectGetterFunc getter) {
    v8::Isolate* isolate = v8::Isolate::GetCurrent();
    v8::HandleScope handle_scope(isolate);
    
    // Create a new variable function callback
    ScriptObjectCallbackPair* pair = new ScriptObjectCallbackPair();
    
    // Copy params
    pair->variableName = name;
    pair->getter = getter;
    
    // Set this up as an accessible variable name
    v8::Local<v8::FunctionTemplate> tpl = m_functionTemplate.Get(isolate);
    tpl->SetNativeDataProperty(v8::String::NewFromUtf8(isolate, name.c_str()), HandleStaticGetter);
    
    m_varList.push_back(pair);
}

void ScriptableObjectType::AddStaticFunction(std::string name, ScriptObjectFunc func) {
    v8::Isolate* isolate = v8::Isolate::GetCurrent();
    v8::HandleScope handle_scope(isolate);
    
    // Create a new variable function callback
    ScriptFunctionCallback* pair = new ScriptFunctionCallback();
    
    // Copy params
    pair->variableName = name;
    pair->func = func;
    
    // Set this up as an accessible variable name
    v8::Local<v8::FunctionTemplate> tpl = m_functionTemplate.Get(isolate);
    tpl->Set(v8::String::NewFromUtf8(isolate, name.c_str()), v8::FunctionTemplate::New(isolate, HandleStaticFunctionCallback));
    
    m_funcList.push_back(pair);
}

void ScriptableObjectType::HandleStaticFunctionCallback(const v8::FunctionCallbackInfo<v8::Value>& info) {
    // Turn our info array into a list of passable Variants
    int argc = info.Length();
    Variant** argv = (Variant**)malloc(sizeof(Variant*) * argc);
    for (int i = 0; i < argc; i++) {
        argv[i] = new ScriptableVariant(info[i]);
    }
    
    // Get the script interface we're supposed to connect to
    ScriptingSystem* ss = GetSystem<ScriptingSystem>();
    
    v8::Local<v8::Object> holder = info.This();
    v8::Local<v8::Function> funcHolder = holder.As<v8::Function>();
    v8::String::Utf8Value thisName(funcHolder->GetName());
    ScriptableObjectType* interface = ss->GetScriptableObjectType(*thisName);
    
    // Iterate through the callback list, looking for a registered callback function
    std::vector<ScriptFunctionCallback*> funcList = interface->GetFunctionList();
    for (size_t i = 0; i < funcList.size(); i++) {
        v8::Local<v8::Function> func = info.Callee();
        v8::String::Utf8Value funcName(func->GetName());
        if (funcList[i]->variableName == std::string(*funcName)) {
            if (funcList[i]->func != 0) {
                ScriptableVariant* scrval = (ScriptableVariant*)(funcList[i]->func(0, argc, argv));
                v8::Local<v8::Value> val = scrval->GetValue();
                info.GetReturnValue().Set(val);
                delete scrval;
                
                break;
            }
        }
    }
    
    // Clean up
    for (int i = 0; i < argc; i++) {
        delete argv[i];
        argv[i] = 0;
    }
    
    free(argv);
}

void ScriptableObjectType::HandleObjectFunctionCallback(const v8::FunctionCallbackInfo<v8::Value>& info) {
    // Unwrap our object
    ScriptableObject* jsobj = ScriptableObject::Unwrap(info.This());
    Variant* obj = new Variant(jsobj);
    
    // Get the script interface we're supposed to connect to
    ScriptingSystem* ss = GetSystem<ScriptingSystem>();
    ScriptableObjectType* interface = ss->GetScriptableObjectType(jsobj->GetGigaName());
    
    // Turn our info array into a list of passable Variants
    int argc = info.Length();
    Variant** argv = (Variant**)malloc(sizeof(Variant*) * argc);
    for (int i = 0; i < argc; i++) {
        argv[i] = new ScriptableVariant(info[i]);
    }
    
    // Iterate through the callback list, looking for a registered callback function
    std::vector<ScriptFunctionCallback*> funcList = interface->GetFunctionList();
    for (size_t i = 0; i < funcList.size(); i++) {
        v8::Local<v8::Function> func = info.Callee();
        v8::String::Utf8Value funcName(func->GetName());
        if (funcList[i]->variableName == std::string(*funcName)) {
            if (funcList[i]->func != 0) {
                ScriptableVariant* value = (ScriptableVariant*)(funcList[i]->func(obj, argc, argv));
                v8::Local<v8::Value> val = value->GetValue();
                info.GetReturnValue().Set(val);
                delete value;
                
                break;
            }
        }
    }
    
    // Clean up
    for (int i = 0; i < argc; i++) {
        delete argv[i];
        argv[i] = 0;
    }
    
    free(argv);
    delete obj;
}

void ScriptableObjectType::HandleObjectGetter(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value>& info) {
    // Unwrap our object
    ScriptableObject* jsobj = ScriptableObject::Unwrap(info.This());
    Variant* obj = new Variant(jsobj);
    
    // Get the script interface we're supposed to connect to
    ScriptingSystem* ss = GetSystem<ScriptingSystem>();
    ScriptableObjectType* interface = ss->GetScriptableObjectType(jsobj->GetGigaName());
    
    // Iterate through the callback list, looking for a registered callback function
    std::vector<ScriptObjectCallbackPair*> varList = interface->GetVariableList();
    v8::String::Utf8Value propName(property);
    for (size_t i = 0; i < varList.size(); i++) {
        std::string strPropName = std::string(*propName);
        if (varList[i]->variableName == strPropName) {
            if (varList[i]->getter != 0) {
                ScriptableVariant* value = (ScriptableVariant*)(varList[i]->getter(strPropName, obj));
                v8::Local<v8::Value> val = value->GetValue();
                info.GetReturnValue().Set(val);
                delete value;
                
                break;
            }
        }
    }
    
    delete obj;
}

void ScriptableObjectType::HandleObjectSetter(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info) {
    ScriptableObject* jsobj = ScriptableObject::Unwrap(info.This());
    Variant* obj = new Variant(jsobj);
    
    // Get the script interface we're supposed to connect to
    ScriptingSystem* ss = GetSystem<ScriptingSystem>();
    ScriptableObjectType* interface = ss->GetScriptableObjectType(jsobj->GetGigaName());
    
    // Iterate through the callback list, looking for a registered callback function
    std::vector<ScriptObjectCallbackPair*> varList = interface->GetVariableList();
    for (size_t i = 0; i < varList.size(); i++) {
        v8::String::Utf8Value propName(property);
        std::string strPropName = std::string(*propName);
        if (varList[i]->variableName == strPropName) {
            if (varList[i]->setter != 0) {
                Variant* sv = new ScriptableVariant(value);
                varList[i]->setter(strPropName, obj, sv);
                delete sv;
                
                break;
            }
        }
    }
    
    delete obj;
}

void ScriptableObjectType::HandleStaticGetter(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value>& info) {
    // Get the script interface we're supposed to connect to
    ScriptingSystem* ss = GetSystem<ScriptingSystem>();
    
    v8::Local<v8::Object> holder = info.This();
    v8::Local<v8::Function> funcHolder = holder.As<v8::Function>();
    v8::String::Utf8Value thisName(funcHolder->GetName());
    ScriptableObjectType* interface = ss->GetScriptableObjectType(*thisName);
    
    // Iterate through the callback list, looking for a registered callback function
    std::vector<ScriptObjectCallbackPair*> varList = interface->GetVariableList();
    for (size_t i = 0; i < varList.size(); i++) {
        v8::String::Utf8Value propName(property);
        std::string strPropName = std::string(*propName);
        if (varList[i]->variableName == strPropName) {
            if (varList[i]->getter != 0) {
                ScriptableVariant* value = (ScriptableVariant*)(varList[i]->getter(strPropName, 0));
                v8::Local<v8::Value> val = value->GetValue();
                info.GetReturnValue().Set(val);
                delete value;
                
                return;
            }
        }
    }
}

void ScriptableObjectType::AddToContext(v8::Local<v8::Context> context) {
    // Inject this type name into V8
    v8::Isolate* isolate = v8::Isolate::GetCurrent();
    v8::Local<v8::FunctionTemplate> tpl = m_functionTemplate.Get(isolate);
    v8::Local<v8::Object> global = isolate->GetCurrentContext()->Global();
    
    // Get our constructor for this type
    m_constructor.Reset(isolate, tpl->GetFunction());
    v8::Local<v8::Function> constructor = m_constructor.Get(isolate);
    
    global->Set(v8::String::NewFromUtf8(isolate, m_typeName.c_str()), constructor);
}

v8::Local<v8::Value> ScriptableObjectType::CreateJSObject() {
    v8::Isolate* isolate = v8::Isolate::GetCurrent();
    
    v8::EscapableHandleScope handle_scope(isolate);
    v8::Local<v8::Function> constructor = m_constructor.Get(isolate);
    
    v8::Local<v8::Object> obj = constructor->NewInstance();
    return(handle_scope.Escape(obj));
}
