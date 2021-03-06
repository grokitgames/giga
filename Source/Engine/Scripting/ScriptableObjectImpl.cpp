
#include <giga-engine.h>

ScriptableObjectImpl::ScriptableObjectImpl() {
	m_type = 0;
}

void ScriptableObjectImpl::Create(ScriptableObjectType* type, ScriptThread* thread) {
	m_type = type;
    m_thread = thread;

	// Start template
    v8::Isolate* isolate = thread->GetIsolate();
	v8::HandleScope handle_scope(isolate);

	v8::Local<v8::FunctionTemplate> tpl = v8::FunctionTemplate::New(isolate, New);
	m_functionTemplate.Reset(isolate, tpl);

	// Add functions
	std::vector<ScriptableObjectType::ScriptFunctionCallback*> functions = type->GetFunctionList();
	for (size_t i = 0; i < functions.size(); i++) {
		if (functions[i]->isStatic == false) {
			tpl->InstanceTemplate()->Set(v8::String::NewFromUtf8(isolate, functions[i]->funcName.c_str()), v8::FunctionTemplate::New(isolate, HandleObjectFunctionCallback));
		}
		else {
			tpl->Set(v8::String::NewFromUtf8(isolate, functions[i]->funcName.c_str()), v8::FunctionTemplate::New(isolate, HandleStaticFunctionCallback));
		}
	}

	// Add variables
	std::vector<ScriptableObjectType::ScriptObjectCallbackPair*> variables = type->GetVariableList();
	for (size_t i = 0; i < variables.size(); i++) {
		if (variables[i]->isStatic == false) {
			tpl->InstanceTemplate()->SetAccessor(v8::String::NewFromUtf8(isolate, variables[i]->variableName.c_str()), HandleObjectGetter, HandleObjectSetter);
		}
		else {
			tpl->SetNativeDataProperty(v8::String::NewFromUtf8(isolate, variables[i]->variableName.c_str()), HandleStaticGetter);
		}
	}

	// End template
	tpl->InstanceTemplate()->SetInternalFieldCount(1);

	// Set our class name
	tpl->SetClassName(v8::String::NewFromUtf8(isolate, m_type->GetName().c_str()));
}

void ScriptableObjectImpl::New(const v8::FunctionCallbackInfo<v8::Value>& info) {
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
	obj->LockMutex();
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

	obj->UnlockMutex();
	return info.GetReturnValue().Set(info.This());
}

void ScriptableObjectImpl::HandleStaticFunctionCallback(const v8::FunctionCallbackInfo<v8::Value>& info) {
	// Turn our info array into a list of passable Variants
	int argc = info.Length();
	Variant** argv = (Variant**)malloc(sizeof(Variant*) * argc);
	for (int i = 0; i < argc; i++) {
		argv[i] = new ScriptableVariant(info[i]);
	}
    
    // Get current thread out of isolate
    v8::Isolate* isolate = v8::Isolate::GetCurrent();
    ScriptThread* thread = (ScriptThread*)isolate->GetData(0);
    ScriptComponent* component = thread->GetCurrentScript();

	// Get the script interface we're supposed to connect to
	ScriptingSystem* ss = GetSystem<ScriptingSystem>();

	v8::Local<v8::Object> holder = info.This();
	v8::Local<v8::Function> funcHolder = holder.As<v8::Function>();
	v8::String::Utf8Value thisName(funcHolder->GetName());
	ScriptableObjectType* interface = ss->GetScriptableObjectType(*thisName);

	if (interface->m_staticObject) {
		interface->m_staticObject->LockMutex();
	}

	// Iterate through the callback list, looking for a registered callback function
	std::vector<ScriptableObjectType::ScriptFunctionCallback*> funcList = interface->GetFunctionList();
	for (size_t i = 0; i < funcList.size(); i++) {
		v8::Local<v8::Function> func = info.Callee();
		v8::String::Utf8Value funcName(func->GetName());
		if (funcList[i]->funcName == std::string(*funcName)) {
			if (funcList[i]->func != 0) {
                ScriptableVariant* scrval = 0;
                if(funcList[i]->isComponent == false) {
                    scrval = (ScriptableVariant*)(funcList[i]->func(0, argc, argv));
                }
                else {
                    scrval = (ScriptableVariant*)(funcList[i]->func2(component, 0, argc, argv));
                }
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

	if (interface->m_staticObject) {
		interface->m_staticObject->UnlockMutex();
	}

	free(argv);
}

void ScriptableObjectImpl::HandleObjectFunctionCallback(const v8::FunctionCallbackInfo<v8::Value>& info) {
	// Unwrap our object
	ScriptableObject* jsobj = ScriptableObject::Unwrap(info.This());
	Variant* obj = new Variant(jsobj);
	jsobj->LockMutex();
    
    // Get current thread out of isolate
    v8::Isolate* isolate = v8::Isolate::GetCurrent();
    ScriptThread* thread = (ScriptThread*)isolate->GetData(0);
    ScriptComponent* component = thread->GetCurrentScript();

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
	std::vector<ScriptableObjectType::ScriptFunctionCallback*> funcList = interface->GetFunctionList();
	for (size_t i = 0; i < funcList.size(); i++) {
		v8::Local<v8::Function> func = info.Callee();
		v8::String::Utf8Value funcName(func->GetName());
		if (funcList[i]->funcName == std::string(*funcName)) {
			if (funcList[i]->func != 0) {
                ScriptableVariant* value = 0;
                if(funcList[i]->isComponent == false) {
                    value = (ScriptableVariant*)(funcList[i]->func(obj, argc, argv));
                }
                else {
                    value = (ScriptableVariant*)(funcList[i]->func2(component, obj, argc, argv));
                }
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

	jsobj->UnlockMutex();
	free(argv);
	delete obj;
}

void ScriptableObjectImpl::HandleObjectGetter(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value>& info) {
	// Unwrap our object
	ScriptableObject* jsobj = ScriptableObject::Unwrap(info.This());
	Variant* obj = new Variant(jsobj);
	jsobj->LockMutex();

	// Get the script interface we're supposed to connect to
	ScriptingSystem* ss = GetSystem<ScriptingSystem>();
	ScriptableObjectType* interface = ss->GetScriptableObjectType(jsobj->GetGigaName());

	// Iterate through the callback list, looking for a registered callback function
	std::vector<ScriptableObjectType::ScriptObjectCallbackPair*> varList = interface->GetVariableList();
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

	jsobj->UnlockMutex();
	delete obj;
}

void ScriptableObjectImpl::HandleObjectSetter(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info) {
	ScriptableObject* jsobj = ScriptableObject::Unwrap(info.This());
	Variant* obj = new Variant(jsobj);
	jsobj->LockMutex();

	// Get the script interface we're supposed to connect to
	ScriptingSystem* ss = GetSystem<ScriptingSystem>();
	ScriptableObjectType* interface = ss->GetScriptableObjectType(jsobj->GetGigaName());

	// Iterate through the callback list, looking for a registered callback function
	std::vector<ScriptableObjectType::ScriptObjectCallbackPair*> varList = interface->GetVariableList();
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

	jsobj->UnlockMutex();
	delete obj;
}

void ScriptableObjectImpl::HandleStaticGetter(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value>& info) {
	// Get the script interface we're supposed to connect to
	ScriptingSystem* ss = GetSystem<ScriptingSystem>();

	v8::Local<v8::Object> holder = info.This();
	v8::Local<v8::Function> funcHolder = holder.As<v8::Function>();
	v8::String::Utf8Value thisName(funcHolder->GetName());
	ScriptableObjectType* interface = ss->GetScriptableObjectType(*thisName);

	if (interface->m_staticObject) {
		interface->m_staticObject->LockMutex();
	}

	// Iterate through the callback list, looking for a registered callback function
	std::vector<ScriptableObjectType::ScriptObjectCallbackPair*> varList = interface->GetVariableList();
	for (size_t i = 0; i < varList.size(); i++) {
		v8::String::Utf8Value propName(property);
		std::string strPropName = std::string(*propName);
		if (varList[i]->variableName == strPropName) {
			if (varList[i]->getter != 0) {
				ScriptableVariant* value = (ScriptableVariant*)(varList[i]->getter(strPropName, 0));
				v8::Local<v8::Value> val = value->GetValue();
				info.GetReturnValue().Set(val);
				delete value;

				break;
			}
		}
	}

	if (interface->m_staticObject) {
		interface->m_staticObject->UnlockMutex();
	}
}

void ScriptableObjectImpl::AddToContext(ScriptThread* thread) {
    assert(m_thread == thread);
    
	// Inject this type name into V8
    v8::Isolate* isolate = m_thread->GetIsolate();
	v8::Local<v8::FunctionTemplate> tpl = m_functionTemplate.Get(isolate);
	v8::Local<v8::Object> global = isolate->GetCurrentContext()->Global();

	global->Set(v8::String::NewFromUtf8(isolate, m_type->GetName().c_str()), tpl->GetFunction());
}

v8::Local<v8::Object> ScriptableObjectImpl::CreateJSObject() {
	v8::Isolate* isolate = m_thread->GetIsolate();

	v8::EscapableHandleScope handle_scope(isolate);
	v8::Local<v8::FunctionTemplate> tpl = m_functionTemplate.Get(isolate);
	v8::Local<v8::Function> constructor = tpl->GetFunction();

	v8::Local<v8::Object> obj = constructor->NewInstance();
	return(handle_scope.Escape(obj));
}

std::string ScriptableObjectImpl::GetName() {
    return(m_type->GetName());
}
