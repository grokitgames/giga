
#include <giga-engine.h>

Entity::Entity() {
    m_id = 0;
    m_updates = false;
}

Entity::~Entity() {
    for(size_t i = 0; i < m_components.size(); i++) {
        delete m_components[i];
    }
}

void Entity::AddComponent(Component* component) {
    component->SetParent(this);
	component->MarkUpdated(true);
    component->OnEntityUpdate();
    
    for (size_t i = 0; i < m_components.size(); i++) {
        if (m_components[i] == component) {
            return;
        }
    }
    
    m_components.push_back(component);
}

void Entity::RemoveComponent(Component* component) {
    component->SetParent(0);
    component->OnEntityUpdate();
    
    for (std::vector<Component*>::iterator i = m_components.begin(); i != m_components.end(); i++) {
        if (*i == component) {
            m_components.erase(i);
            return;
        }
    }
}

Variant* Entity::CreateComponent(Variant* object, int argc, Variant** argv) {
	GIGA_ASSERT(argc == 1, "CreateComponent expects one argument.");
	GIGA_ASSERT(argv[0]->IsString(), "First argument should be string component type.");

	Entity* entity = object->AsObject<Entity>();
	Component* component = Component::CreateComponent(argv[0]->AsString());
	entity->AddComponent(component);

	return(new Variant(component));
}

Entity* Entity::Clone() {
    Entity* entity = new Entity();
    entity->m_id = m_id;
    entity->m_name = m_name;

	entity->CopyStorableObject(this);
	entity->SetDataMappings();
    
    return(entity);
}

Component* Entity::FindComponent(int type) {
    for(size_t i = 0; i < m_components.size(); i++) {
        if(m_components[i]->GetTypeID() == type) {
            return(m_components[i]);
        }
    }
    
    return(0);
}

void Entity::SetDataMappings() {
	SetStorableObjectFieldMapping("entity_name", &m_name);
}

Variant* Entity::FindComponent(Variant* object, int argc, Variant** argv) {
    GIGA_ASSERT(argc == 1, "FindComponent expects one argument.");
    GIGA_ASSERT(argv[0]->IsString(), "Argument should be a string.");
    
    // Convert to Entity object
    Entity* entity = object->AsObject<Entity>();
    std::string name = argv[0]->AsString();
    
    std::vector<Component*> components = entity->GetComponents();
    for(size_t i = 0; i < components.size(); i++) {
        if(components[i]->GetGigaName() == name) {
            return(new Variant(components[i]));
        }
    }
    
    return(new Variant(0));
}

Variant* Entity::GetID(std::string prop, Variant* obj) {
	Entity* entity = obj->AsObject<Entity>();
	return(new Variant(entity->GetID()));
}