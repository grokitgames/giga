
#include <giga-engine.h>

ScriptComponent::ScriptComponent() {
	m_initialized = false;
	m_scriptSource = 0;
}

ScriptComponent::~ScriptComponent() {
    for(size_t i = 0; i < m_functions.size(); i++) {
		m_functions[i]->func.Reset();
        delete m_functions[i];
    }

	// Delete event handlers
	for (size_t i = 0; i < m_eventHandlers.size(); i++) {
		delete m_eventHandlers[i];
	}
    
    m_script.Reset();
    m_context.Reset();
}

void ScriptComponent::Initialize(Script* script) {
	if (m_initialized) return;
	m_initialized = true;

    m_scriptSource = script;
    
    ScriptingSystem* scriptingSystem = GetSystem<ScriptingSystem>();
    scriptingSystem->SetCurrentScript(this);
    
    v8::Isolate* isolate = v8::Isolate::GetCurrent();
    
    // Create a stack-allocated handle scope.
    v8::HandleScope handle_scope(isolate);
    
    // Catch any errors the script might throw
    v8::TryCatch try_catch(isolate);
    
	v8::Local<v8::ObjectTemplate> global;
	v8::Local<v8::Context> context;

	// Create a global object template
	global = v8::ObjectTemplate::New(isolate);

	// Get our context
	context = v8::Context::New(isolate, NULL, global);
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
	std::vector<std::string> interfaceNames;
    for(int i = 0; i < interfaces.size(); i++) {
        interfaces[i]->AddToContext(context);
		interfaceNames.push_back(interfaces[i]->GetName());
    }
    
    // Copy source
    std::string source = m_scriptSource->GetString();
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
		if (this->m_parent) {
			Variant* parent = new Variant(this->m_parent);
			SetGlobal("GameObject", parent);
		}

		scriptingSystem->SetCurrentScript(this);

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
        std::string typeStr = *name;
        
        v8::Local<v8::Value> actual = globalSpace->Get(context, v8::String::NewFromUtf8(isolate, typeStr.c_str())).ToLocalChecked();
        
        v8::String::Utf8Value type(actual->TypeOf(isolate));
        typeStr = *type;
        
        // If an undefined variable, add to our list
		std::map<std::string, ScriptableVariant*>::iterator g = globals.find(*name);
		std::vector<std::string>::iterator ii = std::find(interfaceNames.begin(), interfaceNames.end(), *name);
        if(g == globals.end() && ii == interfaceNames.end() && typeStr != "function") {
			ScriptVariable* var = 0;
			for (size_t i = 0; i < m_globals.size(); i++) {
				if (m_globals[i]->name == *name) {
					var = m_globals[i];
				}
			}

			if (var == 0) {
				var = new ScriptVariable();
				var->name = *name;
			}

			var->value.Reset(isolate, actual);

			m_globals.push_back(var);
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

void ScriptComponent::AddToContext(ScriptableObjectType* type) {
	if (m_initialized == false) {
		this->Initialize(m_scriptSource);
	}

    ScriptingSystem* scriptingSystem = GetSystem<ScriptingSystem>();
    scriptingSystem->SetCurrentScript(this);
    
    v8::Isolate* isolate = v8::Isolate::GetCurrent();
    
    // Create a stack-allocated handle scope.
    v8::HandleScope handle_scope(isolate);
    
    // Catch any errors the script might throw
    v8::TryCatch try_catch(isolate);
    
    // Get our context
    v8::Local<v8::Context> context = m_context.Get(isolate);
    context->Enter();
    
    // Register stuff into our script's context
    type->AddToContext(context);
    
    context->Exit();
    scriptingSystem->SetCurrentScript(0);
}

void ScriptComponent::SetGlobal(std::string name, Variant* value) {
	if (m_initialized == false) {
		this->Initialize(m_scriptSource);
	}

    ScriptingSystem* scriptingSystem = GetSystem<ScriptingSystem>();
    scriptingSystem->SetCurrentScript(this);
    
    v8::Isolate* isolate = v8::Isolate::GetCurrent();
    
    // Create a stack-allocated handle scope.
    v8::HandleScope handle_scope(isolate);
    
    // Reset our current local context
    v8::Local<v8::Context> context = m_context.Get(isolate);
    context->Enter();
    
    // Catch any errors the script might throw
    v8::TryCatch try_catch(isolate);
    
    // Get into our global namespace
    v8::Local<v8::Object> globalSpace = isolate->GetCurrentContext()->Global();
    
	ScriptVariable* var = 0;
	for (size_t i = 0; i < m_globals.size(); i++) {
		if (m_globals[i]->name == name) {
			var = m_globals[i];
		}
	}

	if (var == 0) {
		var = new ScriptVariable();
		var->name = name;
		m_globals.push_back(var);
	}

	ScriptableVariant* sv = (ScriptableVariant*)value;
	var->value.Reset(isolate, sv->GetValue());
	globalSpace->Set(v8::String::NewFromUtf8(isolate, name.c_str()), sv->GetValue());
    
    // Exit
    context->Exit();
    scriptingSystem->SetCurrentScript(0);
}

void ScriptComponent::CallFunction(std::string function, int argc, Variant** argv) {
	if (m_initialized == false) {
		this->Initialize(m_scriptSource);
	}

	// Check to ensure this function exists
	bool exists = false;
	for (size_t i = 0; i < m_functions.size(); i++) {
		if (m_functions[i]->name == function) {
			exists = true;
		}
	}

	if (exists == false) return;

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
    
    // Convert our arguments to v8::Values
    v8::Local<v8::Value>* args = (v8::Local<v8::Value>*)malloc(argc * sizeof(v8::Local<v8::Value>*));
    for(int i = 0; i < argc; i++) {
        ScriptableVariant* variant = (ScriptableVariant*)(argv[i]);
        args[i] = variant->GetValue();
    }

	// Set globals
	for (size_t i = 0; i < m_globals.size(); i++) {
		//globalSpace->Set(v8::String::NewFromUtf8(isolate, m_globals[i]->name.c_str()), m_globals[i]->value.Get(isolate));
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
            GIGA_ASSERT(false, *error);
        }
    }
    else {
        ErrorSystem::Process(new Error(ERROR_WARN, (char*)"Invalid function name", function));
    }

	// Get globals, reset
	for (size_t i = 0; i < m_globals.size(); i++) {
		v8::Local<v8::Value> value = globalSpace->Get(context, v8::String::NewFromUtf8(isolate, m_globals[i]->name.c_str())).ToLocalChecked();
		m_globals[i]->value.Reset(isolate, value);
	}
    
    free(args);
    
    // Exit context
    context->Exit();
    
    scriptingSystem->SetCurrentScript(0);
}

void ScriptComponent::Copy(Component* component) {
    ScriptComponent* clone = (ScriptComponent*)component;

	if(clone->m_scriptSource && m_scriptSource) {
		if (clone->m_scriptSource->GetResource()->filename == m_scriptSource->GetResource()->filename) {
			return;
		}
	}

	clone->m_scriptSource = m_scriptSource;
	clone->m_context = m_context;
	clone->m_script = m_script;
	clone->m_initialized = m_initialized;

	ScriptingSystem* scriptingSystem = GetSystem<ScriptingSystem>();
	scriptingSystem->SetCurrentScript(this);

	v8::Isolate* isolate = v8::Isolate::GetCurrent();

	// Create a stack-allocated handle scope.
	v8::HandleScope handle_scope(isolate);

	// Catch any errors the script might throw
	v8::TryCatch try_catch(isolate);

	for (size_t i = 0; i < m_globals.size(); i++) {
		// Get our context
		v8::Local<v8::Context> context = m_context.Get(isolate);

		// Enter our context
		context->Enter();

		// Get our global object space and start adding stuff to it
		v8::Local<v8::Object> globalSpace = isolate->GetCurrentContext()->Global();

		clone->m_globals.push_back(m_globals[i]);
		v8::Local<v8::Value> value = globalSpace->Get(v8::String::NewFromUtf8(isolate, m_globals[i]->name.c_str()));

		// Exit context
		context->Exit();

		context = clone->m_context.Get(isolate);

		// Enter our context
		context->Enter();
		globalSpace = isolate->GetCurrentContext()->Global();

		// Set
		globalSpace->Set(v8::String::NewFromUtf8(isolate, m_globals[i]->name.c_str()), value);

		context->Exit();
	}

	for (size_t i = 0; i < m_functions.size(); i++) {
		ScriptFunction* func = new ScriptFunction();
		func->func = m_functions[i]->func;
		func->name = m_functions[i]->name;

		clone->m_functions.push_back(func);
	}

	for (size_t i = 0; i < m_eventHandlers.size(); i++) {
		ScriptEventHandler* handler = new ScriptEventHandler();
		handler->type = m_eventHandlers[i]->type;
		handler->entityID = m_eventHandlers[i]->entityID;
		handler->func = m_eventHandlers[i]->func;

		clone->m_eventHandlers.push_back(handler);
	}

	scriptingSystem->SetCurrentScript(0);
}

void ScriptComponent::SetDataMappings() {
	SetStorableObjectFieldMapping("script", (ResourceObject**)&m_scriptSource);
}

void ScriptComponent::RegisterEventHandler(std::string type, std::string func, int entityID) {
	// Create a new event handler
	ScriptEventHandler* handler = new ScriptEventHandler();
	handler->type = type;
	handler->entityID = entityID;
	handler->func = func;

	m_eventHandlers.push_back(handler);
}

void ScriptComponent::ProcessEvent(Event* ev) {
	std::string type = ev->GetType();
	for (size_t i = 0; i < m_eventHandlers.size(); i++) {
		if (m_eventHandlers[i]->type == type) {
			if (m_eventHandlers[i]->entityID == 0 || m_eventHandlers[i]->entityID == ev->GetEntityID()) {
				Variant* v = new Variant(ev);
				Variant* parent = new Variant(this->GetParent());
				SetGlobal("GameObject", parent);
				CallFunction(m_eventHandlers[i]->func, 1, &v);
				delete v;
				delete parent;
			}
		}
	}
}