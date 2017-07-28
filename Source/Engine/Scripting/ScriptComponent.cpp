
#include <giga-engine.h>

ScriptComponent::~ScriptComponent() {
    for(size_t i = 0; i < m_functions.size(); i++) {
        delete m_functions[i];
    }
}

void ScriptComponent::Initialize(Script* script) {
    m_scriptSource = script;
    
    ScriptingSystem* scriptingSystem = GetSystem<ScriptingSystem>();
    scriptingSystem->SetCurrentScript(this);
    
    v8::Isolate* isolate = v8::Isolate::GetCurrent();
    
    // Create a stack-allocated handle scope.
    v8::HandleScope handle_scope(isolate);
    
    // Catch any errors the script might throw
    v8::TryCatch try_catch(isolate);
    
    // Create a global object template
    v8::Local<v8::ObjectTemplate> global = v8::ObjectTemplate::New(isolate);
    
    // Get our context
    v8::Local<v8::Context> context = v8::Context::New(isolate, NULL, global);
    m_context.Reset(isolate, context);
    
    context->Enter();
    
    // Get our global object space and start adding stuff to it
    v8::Local<v8::Object> globalSpace = isolate->GetCurrentContext()->Global();
    
    std::map<std::string, ScriptableVariant*> globals = scriptingSystem->GetRegisteredGlobals();
    for(std::map<std::string, ScriptableVariant*>::iterator i = globals.begin(); i != globals.end(); i++) {
        globalSpace->Set(v8::String::NewFromUtf8(isolate, i->first.c_str()), i->second->GetValue());
    }
    
    // Register stuff into our script's context
    std::vector<ScriptableObjectType*> interfaces = scriptingSystem->GetRegisteredTypes();
    for(int i = 0; i < interfaces.size(); i++) {
        interfaces[i]->AddToContext(context);
    }
    
    // Copy source
    std::string source = (char*)m_scriptSource->GetData();
    v8::Local<v8::String> scriptSrc = v8::String::NewFromUtf8(isolate, source.c_str(), v8::NewStringType::kNormal).ToLocalChecked();
    
    v8::Local<v8::Script> vscript;
    if (v8::Script::Compile(context, scriptSrc).ToLocal(&vscript) == false) {
        v8::String::Utf8Value error(try_catch.Exception());
        ErrorSystem::Process(new Error(ERROR_WARN, (char*)"Unable to compile script file", (char*)*error));
        scriptingSystem->SetCurrentScript(0);
        return;
    }
    
    // Save script
    m_script.Reset(isolate, vscript);
    
    v8::Local<v8::Value> result;
    if (vscript->Run(context).ToLocal(&result) == false) {
        v8::String::Utf8Value error(try_catch.Exception());
        ErrorSystem::Process(new Error(ERROR_WARN, (char*)"Unable to execute script file", (char*)*error));
        scriptingSystem->SetCurrentScript(0);
        return;
    }
    
    // Get the init function
    v8::Local<v8::Value> value = globalSpace->Get(v8::String::NewFromUtf8(isolate, "Init"));
    if(value->IsFunction()) {
        // Call the Init function if there is one
        v8::Local<v8::Function> initFunc = value.As<v8::Function>();
        result = initFunc->Call(globalSpace, 0, 0);
        
        // Attempt to get an error message (need a better way to do this)
        v8::String::Utf8Value error(try_catch.Exception());
        if(*error) {
            ErrorSystem::Process(new Error(ERROR_WARN, (char*)"Unable to execute init function", (char*)*error));
        }
    }
    
    // Loop through and find any global variable names
    v8::Local<v8::Array> vars = globalSpace->GetPropertyNames(context).ToLocalChecked();
    int sizearr = vars->Length();
    for(int i = 0; i < sizearr; i++) {
        v8::Local<v8::Value> var = vars->Get(i);
        
        v8::String::Utf8Value name(var->ToString(isolate));
        char* typeStr = (char*)*name;
        
        v8::Local<v8::Value> actual = globalSpace->Get(context, v8::String::NewFromUtf8(isolate, typeStr)).ToLocalChecked();
        
        v8::String::Utf8Value type(actual->TypeOf(isolate));
        typeStr = (char*)*type;
        
        // If an undefined variable, add to our list
        if(strcmp(typeStr, "undefined")) {
            m_globals.push_back(std::string(typeStr));
        }
        
        if(actual->IsFunction()) {
            v8::Local<v8::Function> func = actual.As<v8::Function>();
            v8::String::Utf8Value funcName(func->GetName());
            
            ScriptFunction* cached = new ScriptFunction();
            cached->name = std::string(*funcName);
            cached->func.Reset(isolate, func);
            
            m_functions.push_back(cached);
        }
    }
    
    context->Exit();
    scriptingSystem->SetCurrentScript(0);
}

void ScriptComponent::CallFunction(std::string function, int argc, Variant** argv) {
    ScriptingSystem* scriptingSystem = GetSystem<ScriptingSystem>();
    scriptingSystem->SetCurrentScript(this);
    
    v8::Isolate* isolate = v8::Isolate::GetCurrent();
    
    // Create a stack-allocated handle scope.
    v8::HandleScope handle_scope(isolate);
    
    // Catch any errors the script might throw
    v8::TryCatch try_catch(isolate);
    
    // Get our context
    v8::Local<v8::Context> context = m_context.Get(isolate);
    
    // Enter our context
    context->Enter();
    
    // Get our global object space and start adding stuff to it
    v8::Local<v8::Object> globalSpace = isolate->GetCurrentContext()->Global();
    
    // Convert our arguments to v8::values
    v8::Local<v8::Value>* args = (v8::Local<v8::Value>*)malloc(argc * sizeof(v8::Local<v8::Value>*));
    for(int i = 0; i < argc; i++) {
        args[i] = dynamic_cast<ScriptableVariant*>(argv[i])->GetValue();
    }
    
    // Attempt to call the function
    v8::Local<v8::Value> value = globalSpace->Get(v8::String::NewFromUtf8(isolate, function.c_str()));
    if(value->IsFunction()) {
        // Call the Init function if there is one
        v8::Local<v8::Function> initFunc = value.As<v8::Function>();
        v8::Local<v8::Value> result = initFunc->Call(globalSpace, argc, args);
        
        // Attempt to get an error message (need a better way to do this)
        v8::String::Utf8Value error(try_catch.Exception());
        if(*error) {
            ErrorSystem::Process(new Error(ERROR_WARN, (char*)"Unable to execute function", (char*)*error));
        }
    }
    else {
        ErrorSystem::Process(new Error(ERROR_WARN, (char*)"Invalid function name", function));
    }
    
    // Exit context
    context->Exit();
    
    scriptingSystem->SetCurrentScript(0);
}

ScriptComponent* ScriptComponent::Clone() {
    ScriptComponent* clone = new ScriptComponent();
    clone->Initialize(m_scriptSource);
    return(clone);
}
