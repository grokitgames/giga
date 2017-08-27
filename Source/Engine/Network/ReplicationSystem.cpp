
#include <giga-engine.h>

void ReplicationSystem::Update(float delta) {
	// Make sure the type is set
	if (m_type == 0) {
		return;
	}

	// Get the current tick
	NetworkSystem* networkSystem = GetSystem<NetworkSystem>();
	int tick = networkSystem->GetCurrentTick();

	// Make sure we're in a new tick
	if (tick <= m_lastTick) {
		return;
	}

	// If this is the server, prepare a sync to be sent to clients
	if (m_type == REPLICATION_SERVER) {
		// Get all entities
		EntitySystem* entitySystem = GetSystem<EntitySystem>();
		std::vector<Entity*> entities = entitySystem->GetEntities();

		// Create an entity snapshot
		EntitySnapshot* snapshot = new EntitySnapshot();
		snapshot->tick = tick;

		// Iterate over to find entities with updates
		for (size_t i = 0; i < entities.size(); i++) {
			if (entities[i]->HasUpdates() > 0) {
				// Create a replica of the entity
				Entity* entity = new Entity();
				entity->SetID(entities[i]->GetID());
				entity->SetName(entities[i]->GetName());

				// Find any updated components
				int updatedComponents = 0;
				std::vector<Component*> components = entities[i]->GetComponents();
				for (size_t j = 0; j < components.size(); j++) {
					if (components[j]->HasUpdates()) {
						entity->AddComponent(components[j]->Clone());
						updatedComponents++;
					}
				}

				printf("Adding entity with %d updated component(s).\n", updatedComponents);

				// Add entity to snapshot
				snapshot->entities.push_back(entity);
			}
		}

		// Serialize
		unsigned char* buffer = (unsigned char*)malloc(NETWORK_MAX_PACKET_SIZE);
		int actualSize = NETWORK_MAX_PACKET_SIZE;
		int offset = 0;

		while (offset < snapshot->entities.size()) {
			EntitySnapshotMessage* msg = new EntitySnapshotMessage();
			snapshot->Serialize(buffer, actualSize, offset);
			msg->SetEntityPayload(buffer, actualSize);

			networkSystem->Send(msg);
            delete msg;
		}

		// Save
		AddSnapshot(tick, snapshot);
	}
	else {
		// If this is a client, process any updates that need to happen (need at least 2 snapshots)

        // Adjust our tick by a set amount of "render lag" so that we can interpolate
        int renderTick = tick - NETWORK_SNAPSHOT_RENDER_LAG;
        
        // Check which snapshots we have to interpolate between
        int startTick = 0;
        int endTick = renderTick;
        std::list<EntitySnapshot*>::iterator i = m_snapshots.end();
        std::list<EntitySnapshot*>::iterator i2 = m_snapshots.end();
        
        for(i; i != m_snapshots.begin(); i--) {
            if((*i)->tick <= startTick) {
                startTick = (*i)->tick;
                break;
            }
            endTick = (*i)->tick;
            i2 = i;
        }
        
        if(startTick > 0) {
            // Get the current offset from start tick
            float startTime = startTick / NETWORK_TICKS_PER_SECOND;
            float endTime = endTick / NETWORK_TICKS_PER_SECOND;
            float currentPos = networkSystem->GetCurrentTime() - ((tick - renderTick) / NETWORK_TICKS_PER_SECOND) - startTime;
            float interpolate = currentPos / (endTime - startTime);
            
            // Get link to entity system
            EntitySystem* entitySystem = GetSystem<EntitySystem>();
            
            // Apply changes from the startTick (now in i)
            for(size_t j = 0; j < (*i)->entities.size(); j++) {
                Entity* entity = entitySystem->FindEntity((*i)->entities[j]->GetID());
                if(entity == 0) {
                    entity = new Entity();
                    entity->SetID((*i)->entities[j]->GetID());
                }
                
                std::vector<Component*> components = (*i)->entities[j]->GetComponents();
                for(size_t k = 0; k < components.size(); k++) {
                    Component* component = entity->FindComponent(components[k]->GetTypeID());
                    if(component) {
                        entity->RemoveComponent(component);
                        delete component;
                    }
                    
                    component = components[k]->Clone();
                    component->SetDataMappings();
                    
                    // If this same entity is present in the next snapshot, interpolate
                    for(size_t m = 0; m < (*i2)->entities.size(); m++) {
                        if((*i2)->entities[m]->GetID() == (*i)->entities[j]->GetID()) {
                            Component* updated = (*i2)->entities[m]->FindComponent(components[k]->GetTypeID());
                            if(updated) {
                                component->Interpolate(updated, interpolate);
                            }
                        }
                    }
                    
                    entity->AddComponent(component);
                }
            }
        }
	}

	// Finally, delete any "old" snapshots that are no longer needed
	if (m_snapshots.size()) {
		std::list<EntitySnapshot*>::iterator i = m_snapshots.begin();
		if ((*i)->tick < (tick - NETWORK_SNAPSHOT_HISTORY)) {
			for (i; i != m_snapshots.end(); i++) {
				if ((*i)->tick > (tick - NETWORK_SNAPSHOT_HISTORY))
					break;
			}

			m_snapshots.erase(m_snapshots.begin(), i);
		}
	}

	// Save current tick as most recent processed
	m_lastTick = tick;
}

void ReplicationSystem::AddSnapshot(int tick, EntitySnapshot* snapshot) {
	// Check for empty list
	if (m_snapshots.size() == 0) {
		m_snapshots.push_back(snapshot);
		return;
	}

	// Check if this snapshot just goes at the end
	std::list<EntitySnapshot*>::iterator i = m_snapshots.end();
	i--;

	if ((*i)->tick < tick) {
		m_snapshots.push_back(snapshot);
		return;
	}

	// Otherwise, go through the list and find out where to insert
	for (i; i != m_snapshots.begin(); i--) {
		if (tick > (*i)->tick) {
			m_snapshots.insert(i, snapshot);
			return;
		}
	}

	// If we get here, we to put it at the front of the list
	m_snapshots.push_front(snapshot);
}
