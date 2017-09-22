
#include <giga-engine.h>

Entity* EntitySystem::CreateEntity(std::string name) {
    // Create new entity, assigning name and ID
    Entity* entity = new Entity();
    entity->SetName(name);
    entity->SetID(++m_lastEntityID);
    
    // Add to our pool
    m_entities.AddObject(entity);
    
    return(entity);
}

void EntitySystem::AddEntity(Entity* entity) {
    // Assign entity ID if it doesn't have one
    if(entity->GetID() == 0) {
        entity->SetID(++m_lastEntityID);
    }
    else {
        // Increment our own internal counter to ensure it is higher
        m_lastEntityID = std::max(entity->GetID(), m_lastEntityID);
    }
    
    // Add to our pool
    m_entities.AddObject(entity);
}

void EntitySystem::RemoveEntity(Entity* entity) {
    m_entities.RemoveObject(entity);
}

void EntitySystem::Clear() {
    m_entities.Clear();
}

Entity* EntitySystem::FindEntity(int id) {
    std::vector<Entity*> entities = m_entities.GetList();
	std::vector<Entity*>::iterator i = entities.begin();
	for (i; i != entities.end(); i++) {
        if((*i)->GetID() == id) {
            return((*i));
        }
    }
    
    return(0);
}

Entity* EntitySystem::FindEntity(std::string name) {
	std::vector<Entity*> entities = m_entities.GetList();
	std::vector<Entity*>::iterator i = entities.begin();
	for (i; i != entities.end(); i++) {
        if((*i)->GetName() == name) {
            return((*i));
        }
    }
    
    return(0);
}

Variant* EntitySystem::FindEntity(Variant* object, int argc, Variant** argv) {
    // Make sure we have either a string name or an integer ID
    GIGA_ASSERT(argc == 1, "FindEntity function expects a single parameter.");
    GIGA_ASSERT(argv[0]->IsInt() || argv[0]->IsString(), "Parameter should be string name or int ID.");
    
    // Get our entity system
    EntitySystem* entitySystem = GetSystem<EntitySystem>();
    
    // Search by name for a string
    if(argv[0]->IsString()) {
        return(new Variant(entitySystem->FindEntity(argv[0]->AsString())));
    }
    
    if(argv[0]->IsInt()) {
        return(new Variant(entitySystem->FindEntity(argv[0]->AsInt())));
    }
    
    return(new Variant(0));
}
