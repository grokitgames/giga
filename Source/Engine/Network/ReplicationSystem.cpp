
#include <giga-engine.h>

void ReplicationSystem::Update(float delta) {
    // Make sure the type is set
    if(m_type == 0) {
        return;
    }
    
    // Get the current tick
    NetworkSystem* networkSystem = GetSystem<NetworkSystem>();
    int tick = networkSystem->GetCurrentTick();
    
    // Make sure we're in a new tick
    if(tick <= m_lastTick) {
        return;
    }
    
    // If this is the server, prepare a sync to be sent to clients
    if(m_type == REPLICATION_SERVER) {
        // Get all entities
        EntitySystem* entitySystem = GetSystem<EntitySystem>();
        std::vector<Entity*> entities = entitySystem->GetEntities();
        
        // Create an entity snapshot
        EntitySnapshot* snapshot = new EntitySnapshot();
        snapshot->tick = tick;
        
        // Iterate over to find entities with updates
        for(size_t i = 0; i < entities.size(); i++) {
            if(entities[i]->HasUpdates() > 0) {
                // Create a replica of the entity
                Entity* entity = new Entity();
                entity->SetID(entities[i]->GetID());
                entity->SetName(entities[i]->GetName());
                
                // Find any updated components
                std::vector<Component*> components = entity->GetComponents();
                for(size_t j = 0; j < components.size(); j++) {
                    if(components[j]->HasUpdates()) {
                        entity->AddComponent(components[j]->Clone());
                    }
                }
                
                // Add entity to snapshot
                snapshot->entities.push_back(entity);
            }
        }
        
        // Serialize
        unsigned char* buffer = (unsigned char*)malloc(NETWORK_MAX_PACKET_SIZE);
        int actualSize = NETWORK_MAX_PACKET_SIZE;
        int offset = 0;
        
        while(offset < snapshot->entities.size()) {
            EntitySnapshotMessage* msg = new EntitySnapshotMessage();
            snapshot->Serialize(buffer, actualSize, offset);
            msg->SetEntityPayload(buffer, actualSize);
            
            networkSystem->Send(msg);
        }
    }
    else {
        // If this is a client, process any updates that need to happen
    }
    
    // Save current tick as most recent processed
    m_lastTick = tick;
}
