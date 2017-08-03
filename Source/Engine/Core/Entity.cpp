
#include <giga-engine.h>

Entity::Entity() {
    m_id = 0;
}

Entity::~Entity() {
    for(size_t i = 0; i < m_components.size(); i++) {
        delete m_components[i];
    }
}

void Entity::AddComponent(Component* component) {
    component->SetParent(this);
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

Entity* Entity::Clone() {
    Entity* entity = new Entity();
    entity->m_id = m_id;
    entity->m_name = m_name;
    
    for(size_t i = 0; i < m_components.size(); i++) {
        entity->AddComponent(m_components[i]->Clone());
    }
    
    return(entity);
}

Variant* Entity::FindComponent(Variant* object, int argc, Variant** argv) {
    GIGA_ASSERT(argc == 1, "FindComponent expects one argument.");
    GIGA_ASSERT(argv[0]->IsString(), "Argument should be a string.");
    
    // Convert to Entity object
    Entity* entity = object->AsObject<Entity>();
    std::string name = argv[0]->AsString();
    
    std::vector<Component*> components = entity->GetComponents();
    for(size_t i = 0; components.size(); i++) {
        if(components[i]->GetClassName() == name) {
            return(new Variant(components[i]));
        }
    }
    
    return(new Variant(0));
}
