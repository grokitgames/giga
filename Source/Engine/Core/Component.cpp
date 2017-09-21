
#include <giga-engine.h>

std::map<std::string, ComponentType*> Component::m_componentTypes;

Component::Component() {
    m_parent = 0;
    m_active = false;
    m_updated = false;
    m_typeID = 0;
	m_removeFunction = 0;
	m_addFunction = 0;
}

Component::~Component() {
    // Call the necessary system to remove component from any lists
	if (m_removeFunction) {
		m_removeFunction(this);
	}
    else {
		// GIGA_ASSERT(false, "Component removal function undefined.");
    }
}

Component* Component::CreateComponent(std::string type) {
    // Find component creation function
    std::map<std::string, ComponentType*>::iterator i = m_componentTypes.find(type);
    if (i == m_componentTypes.end()) {
        // Component type not registered
		GIGA_ASSERT(false, "Type not found.");
        return(0);
    }
    
    Component* component = i->second->createFunc();
	component->InitializeComponent(type);
	component->m_addFunction = i->second->addFunc;
    component->m_removeFunction = i->second->removeFunc;
    component->m_typeID = i->second->typeID;
    return(component);
}

Component* Component::CreateComponent(int typeID) {
    // Find component creation function
    std::map<std::string, ComponentType*>::iterator i = m_componentTypes.begin();
    for(i; i != m_componentTypes.end(); i++) {
        if(i->second->typeID == typeID) {
            Component* component = i->second->createFunc();
			component->InitializeComponent(i->second->name);

			component->m_addFunction = i->second->addFunc;
            component->m_removeFunction = i->second->removeFunc;
            component->m_typeID = i->second->typeID;
            return(component);
        }
    }
    
	GIGA_ASSERT(false, "Type not found.");
    return(0);
}

void Component::InitializeStorableObject(std::string name) {
	// Set functions
	std::map<std::string, ComponentType*>::iterator i = m_componentTypes.begin();
	for (i; i != m_componentTypes.end(); i++) {
		if (i->second->name == name) {
			this->InitializeComponent(i->second->name);

			this->m_addFunction = i->second->addFunc;
			this->m_removeFunction = i->second->removeFunc;
			this->m_typeID = i->second->typeID;

			return;
		}
	}
}

void Component::AddToSystem() {
	if (m_addFunction) {
		m_addFunction(this);
		return;
	}

	int typeID = this->GetTypeID();
	std::map<std::string, ComponentType*>::iterator i = m_componentTypes.begin();
	for (i; i != m_componentTypes.end(); i++) {
		if (i->second->typeID == typeID) {
			m_addFunction = i->second->addFunc;
			m_addFunction(this);
			return;
		}
	}

	GIGA_ASSERT(false, "Unable to add component to system.");
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

Component* Component::Clone() {
	Component* component = Component::CreateComponent(this->GetTypeID());
	component->m_active = m_active;
	component->m_updated = m_updated;

	this->Copy(component);
	component->CopyStorableObject(this);
	component->SetDataMappings();
	return(component);
}

std::vector<std::string> Component::GetComponentTypes() {
	std::vector<std::string> list;
	std::map<std::string, ComponentType*>::iterator i = m_componentTypes.begin();
	for (; i != m_componentTypes.end(); i++) {
		list.push_back(i->first);
	}

	return(list);
}
