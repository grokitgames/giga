
#include <giga-engine.h>

ScriptableObjectType::ScriptableObjectType(std::string name) {
    m_static = false;
    m_transient = false;
    m_staticObject = 0;
    
    SetName(name);
    StartTemplate();
}

void ScriptableObjectType::StartTemplate() {
    
}

void ScriptableObjectType::EndTemplate() {

}

void ScriptableObjectType::AddObjectVariable(std::string name, ScriptObjectGetterFunc getter, ScriptObjectSetterFunc setter) {
    // Create a new variable function callback
    ScriptObjectCallbackPair* pair = new ScriptObjectCallbackPair();
    
    // Copy params
    pair->variableName = name;
    pair->getter = getter;
    pair->setter = setter;
	pair->isStatic = false;
    
    m_varList.push_back(pair);
}

void ScriptableObjectType::AddObjectFunction(std::string name, ScriptObjectFunc func) {
    // Create a new variable function callback
    ScriptFunctionCallback* pair = new ScriptFunctionCallback();
    
    // Copy params
    pair->funcName = name;
    pair->func = func;
	pair->isStatic = false;
    pair->isComponent = false;
    
    m_funcList.push_back(pair);
}

void ScriptableObjectType::AddObjectFunction(std::string name, ScriptComponentObjectFunc func) {
    // Create a new variable function callback
    ScriptFunctionCallback* pair = new ScriptFunctionCallback();
    
    // Copy params
    pair->funcName = name;
    pair->func2 = func;
    pair->isStatic = false;
    pair->isComponent = true;
    
    m_funcList.push_back(pair);
}

void ScriptableObjectType::AddStaticVariable(std::string name, ScriptObjectGetterFunc getter) {
    // Create a new variable function callback
    ScriptObjectCallbackPair* pair = new ScriptObjectCallbackPair();
    
    // Copy params
    pair->variableName = name;
    pair->getter = getter;
	pair->isStatic = true;
    
    m_varList.push_back(pair);
}

void ScriptableObjectType::AddStaticFunction(std::string name, ScriptObjectFunc func) {
    // Create a new variable function callback
    ScriptFunctionCallback* pair = new ScriptFunctionCallback();
    
    // Copy params
    pair->funcName = name;
    pair->func = func;
	pair->isStatic = true;
    pair->isComponent = false;

    m_funcList.push_back(pair);
}

void ScriptableObjectType::AddStaticFunction(std::string name, ScriptComponentObjectFunc func) {
    // Create a new variable function callback
    ScriptFunctionCallback* pair = new ScriptFunctionCallback();
    
    // Copy params
    pair->funcName = name;
    pair->func2 = func;
    pair->isStatic = true;
    pair->isComponent = true;
    
    m_funcList.push_back(pair);
}
