
#include <giga-engine.h>

std::map<std::string, ComponentType*> Component::m_componentTypes;

Component::Component() {
    m_parent = 0;
    m_active = false;
    m_updated = false;
    m_typeID = 0;
	removeFunction = 0;
}

Component::~Component() {
    // Call the necessary system to remove component from any lists
	if (removeFunction) {
		removeFunction(this);
	}
}

void Component::RegisterComponentType(std::string type, int typeID, ComponentCreateFunc f1, ComponentRemoveFunc f2) {
    std::map<std::string, ComponentType*>::iterator i = m_componentTypes.begin();
    for(i; i != m_componentTypes.end(); i++) {
        if(i->second->name == type || i->second->typeID == typeID) {
            GIGA_ASSERT(false, "Component name or type ID already registered.");
        }
    }
    
    ComponentType* ct = new ComponentType();
    ct->name = type;
    ct->typeID = typeID;
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
    
    Component* component = i->second->createFunc(type);
    component->removeFunction = i->second->removeFunc;
    component->m_typeID = i->second->typeID;
    return(component);
}

Component* Component::CreateComponent(int typeID) {
    // Find component creation function
    std::map<std::string, ComponentType*>::iterator i = m_componentTypes.begin();
    for(i; i != m_componentTypes.end(); i++) {
        if(i->second->typeID == typeID) {
            Component* component = i->second->createFunc(i->second->name);
            component->removeFunction = i->second->removeFunc;
            component->m_typeID = i->second->typeID;
            return(component);
        }
    }
    
    return(0);
}

void Component::MarkUpdated(bool updated) {
    // Only process if we're changing state
    if(m_updated == updated) {
        return;
    }
	    
    // If we're setting as updated and we're before, increment component update count on entity
	if (m_parent) {
		if (updated == true && m_updated == false) {
			m_parent->SetUpdated(1);
		}
		// Otherwise, decrement (we are no longer updated)
		else {
			m_parent->SetUpdated(-1);
		}
	}
    
    m_updated = updated;
}

int Component::GetTypeID() {
	if (m_typeID) {
		return(m_typeID);
	}

	std::map<std::string, ComponentType*>::iterator i = m_componentTypes.find(this->GetGigaName());
	GIGA_ASSERT(i != m_componentTypes.end(), "Component type not found.");
	m_typeID = i->second->typeID;
	return(m_typeID);
}