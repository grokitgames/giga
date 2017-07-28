
#include <giga-engine.h>

std::map<std::string, ComponentType*> Component::m_componentTypes;

Component::Component() {
    m_parent = 0;
    m_active = false;
    m_updated = false;
}

Component::~Component() {
    // Call the necessary system to remove component from any lists
    removeFunction(this);
}

void Component::RegisterComponentType(std::string type, ComponentCreateFunc f1, ComponentRemoveFunc f2) {
    std::map<std::string, ComponentType*>::iterator i = m_componentTypes.find(type);
    assert(i == m_componentTypes.end()); // BAD: component type already registered
    
    ComponentType* ct = new ComponentType();
    ct->name = type;
    ct->createFunc = f1;
    ct->removeFunc = f2;
    
    m_componentTypes[type] = ct;
}

Component* Component::CreateComponent(std::string type) {
    // Find component creation function
    std::map<std::string, ComponentType*>::iterator i = m_componentTypes.find(type);
    if (i == m_componentTypes.end()) {
        // Component type not registered
        return(0);
    }
    
    Component* component = i->second->createFunc();
    component->removeFunction = i->second->removeFunc;
    return(component);
}
