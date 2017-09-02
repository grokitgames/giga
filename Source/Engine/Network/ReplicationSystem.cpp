
#include <giga-engine.h>

ReplicationSystem::ReplicationSystem() {
    m_lastTick = 0;
    m_type = 0;
    m_initialized = false;
}

ReplicationSystem::~ReplicationSystem() {
    std::list<EntitySnapshot*>::iterator i = m_snapshots.begin();
    for(i; i != m_snapshots.end(); i++) {
        delete (*i);
    }
}

void ReplicationSystem::Update(float delta) {
	PROFILE_START_AREA("ReplicationSystem Update");

	// Make sure the type is set
	if (m_type == 0) {
		return;
	}

	// Get the current tick
	NetworkSystem* networkSystem = GetSystem<NetworkSystem>();
	int tick = networkSystem->GetCurrentTick();

	// If this is the server, prepare a sync to be sent to clients
	if (m_type == REPLICATION_SERVER) {
		// Make sure we're in a new tick
		if (tick <= m_lastTick) {
			PROFILE_END_AREA("ReplicationSystem Update");
			return;
		}

		// Get all entities
		EntitySystem* entitySystem = GetSystem<EntitySystem>();
		std::list<Entity*> entities = entitySystem->GetEntities();

		// Create an entity snapshot
		EntitySnapshot* snapshot = new EntitySnapshot();
		snapshot->tick = tick;
		snapshot->type = EntitySnapshot::SNAPSHOT_DELTA;

		EntitySnapshot* fullSnapshot = new EntitySnapshot();
		fullSnapshot->tick = tick;
		fullSnapshot->type = EntitySnapshot::SNAPSHOT_FULL;

		// Iterate over to find entities with updates
		std::list<Entity*>::iterator i = entities.begin();
		for (i; i != entities.end(); i++) {
			// First, take full entity snapshot
			Entity* e1 = new Entity();
			e1->SetID((*i)->GetID());
			e1->SetName((*i)->GetName());

			// Add all components
			std::vector<Component*> components = (*i)->GetComponents();
			for (size_t j = 0; j < components.size(); j++) {
				e1->AddComponent(components[j]->Clone());
			}

			fullSnapshot->entities.push_back(e1);

			// Then do delta snapshot
			if ((*i)->HasUpdates() > 0) {
				// Create a replica of the entity
				Entity* entity = new Entity();
				entity->SetID((*i)->GetID());
				entity->SetName((*i)->GetName());

				// Find any updated components
				int updatedComponents = 0;
				for (size_t j = 0; j < components.size(); j++) {
					if (components[j]->HasUpdates()) {
						entity->AddComponent(components[j]->Clone());
						components[j]->MarkUpdated(false);
						updatedComponents++;
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

		while (offset < snapshot->entities.size()) {
			EntitySnapshotMessage* msg = new EntitySnapshotMessage();
			snapshot->Serialize(buffer, actualSize, offset);
			msg->SetEntityPayload(buffer, actualSize);

			networkSystem->Send(msg);
            delete msg;
		}
        
		// Save
		AddSnapshot(tick, snapshot);
		AddFullSnapshot(tick, fullSnapshot);

		// Send our full snapshot to any sessions that need it
		if (m_sessionIDs.size()) {
			EntitySnapshotMessage* msg = new EntitySnapshotMessage();
			unsigned char* packet = (unsigned char*)malloc(NETWORK_MAX_PACKET_SIZE);

			int size = NETWORK_MAX_PACKET_SIZE;
			int offset = 0;
			fullSnapshot->Serialize(packet, size, offset);

			msg->SetEntityPayload(packet, size);
			printf("Sending full snapshot of size %d with %d entities.\n", size, fullSnapshot->entities.size());

			std::list<int>::iterator s = m_sessionIDs.begin();
			for (; s != m_sessionIDs.end(); s++) {
				networkSystem->Send(*s, msg);
			}

			delete msg;
			m_sessionIDs.clear();
		}
	}
	else {
		// If this is a client, process any updates that need to happen

		// Adjust our tick by a set amount of "render lag" so that we can interpolate
		int renderTick = tick - NETWORK_SNAPSHOT_RENDER_LAG;
        
        // If we are not yet initialized, check for a full snapshot first
        if(m_initialized == false && m_snapshots.size() > 0) {
            std::list<EntitySnapshot*>::iterator i = m_snapshots.begin();
            for(i; i != m_snapshots.end(); i++) {
                if((*i)->type == EntitySnapshot::SNAPSHOT_FULL) {
                    ApplySnapshot(*i, 0, 0);
					m_initialized = true;
                }

				if (m_initialized == true) {
					// Apply next snapshots up to expected render tick
					if ((*i)->tick < renderTick) {
						ApplySnapshot(*i, 0, 0);
					}
				}
            }
            
			PROFILE_END_AREA("ReplicationSystem Update");
            return;
        }

		if (m_initialized == false) {
			PROFILE_END_AREA("ReplicationSystem Update");
			return;
		}
        
        // Check which snapshots we have to interpolate between
        int startTick = 0;
        int endTick = renderTick;
        std::list<EntitySnapshot*>::iterator i = m_snapshots.end();
        std::list<EntitySnapshot*>::iterator i2 = m_snapshots.end();
        
		if (m_snapshots.size() > 2) {
			i--;
			for (i; i != m_snapshots.begin(); i--) {
				if ((*i)->tick <= renderTick) {
					startTick = (*i)->tick;
					break;
				}
				endTick = (*i)->tick;
				i2 = i;
			}
		}
        
        if(startTick > 0) {
            // Get the current offset from start tick
            float startTime = (float)startTick / NETWORK_TICKS_PER_SECOND;
            float endTime = (float)endTick / NETWORK_TICKS_PER_SECOND;
            float currentPos = networkSystem->GetCurrentTime() - (((float)tick - renderTick) / NETWORK_TICKS_PER_SECOND) - startTime;
            float interpolate = currentPos / (endTime - startTime);
			if (interpolate < 0) {
				interpolate = 0;
			}
			if (interpolate > 1) {
				interpolate = 1;
			}
            
			if (i2 != m_snapshots.end()) {
				ApplySnapshot(*i, *i2, interpolate);
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
                else
                    delete (*i);
			}

			m_snapshots.erase(m_snapshots.begin(), i);
		}
	}

	// Same thing with full snapshots, delete any "old" snapshots that are no longer needed
	if (m_fullSnapshots.size()) {
		std::list<EntitySnapshot*>::iterator i = m_fullSnapshots.begin();
		if ((*i)->tick < (tick - NETWORK_SNAPSHOT_HISTORY)) {
			for (i; i != m_fullSnapshots.end(); i++) {
				if ((*i)->tick >(tick - NETWORK_SNAPSHOT_HISTORY))
					break;
				else
					delete (*i);
			}

			m_fullSnapshots.erase(m_fullSnapshots.begin(), i);
		}
	}

	// Save current tick as most recent processed
	m_lastTick = tick;

	PROFILE_END_AREA("ReplicationSystem Update");
}

void ReplicationSystem::ApplySnapshot(EntitySnapshot* current, EntitySnapshot* next, float interpolate) {
    // Get link to entity system
    EntitySystem* entitySystem = GetSystem<EntitySystem>();
    
    // Apply changes from the startTick (now in i)
    for(size_t j = 0; j < current->entities.size(); j++) {
        Entity* entity = entitySystem->FindEntity(current->entities[j]->GetID());
        if(entity == 0) {
            entity = new Entity();
            entity->SetID(current->entities[j]->GetID());
            entitySystem->AddEntity(entity);
        }
        
        std::vector<Component*> components = current->entities[j]->GetComponents();
        for(size_t k = 0; k < components.size(); k++) {
            Component* component = entity->FindComponent(components[k]->GetTypeID());
            if(component) {
                entity->RemoveComponent(component);
                delete component;
            }
            
            component = components[k]->Clone();
            component->SetDataMappings();
            component->SetActive(true);
            component->AddToSystem();
            
            // If this same entity is present in the next snapshot, interpolate
            if (next) {
                for (size_t m = 0; m < next->entities.size(); m++) {
                    if (next->entities[m]->GetID() == current->entities[j]->GetID()) {
                        Component* updated = next->entities[m]->FindComponent(components[k]->GetTypeID());
                        if (updated) {
                            component->Interpolate(updated, interpolate);
                        }
                    }
                }
            }
            
            entity->AddComponent(component);
        }
    }
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
        if(tick == (*i)->tick) {
            // If we get an exact match, add to existing list
            (*i)->entities.insert((*i)->entities.end(), snapshot->entities.begin(), snapshot->entities.end());
            delete snapshot;
            return;
        }
        
		if (tick > (*i)->tick) {
			m_snapshots.insert(i, snapshot);
			return;
		}
	}

	// If we get here, we to put it at the front of the list
	m_snapshots.push_front(snapshot);
}

void ReplicationSystem::AddFullSnapshot(int tick, EntitySnapshot* snapshot) {
	// Check for empty list
	if (m_fullSnapshots.size() == 0) {
		m_fullSnapshots.push_back(snapshot);
		return;
	}

	// Check if this snapshot just goes at the end
	std::list<EntitySnapshot*>::iterator i = m_fullSnapshots.end();
	i--;

	if ((*i)->tick < tick) {
		m_fullSnapshots.push_back(snapshot);
		return;
	}

	// Otherwise, go through the list and find out where to insert
	for (i; i != m_fullSnapshots.begin(); i--) {
		if (tick >(*i)->tick) {
			m_fullSnapshots.insert(i, snapshot);
			return;
		}
	}

	// If we get here, we to put it at the front of the list
	m_fullSnapshots.push_front(snapshot);
}

EntitySnapshot* ReplicationSystem::GetEntitySnapshot(int tick) {
	std::list<EntitySnapshot*>::iterator i = m_snapshots.begin();
	for (i; i != m_snapshots.end(); i++) {
		if ((*i)->tick == tick) {
			return(*i);
		}
	}
	
	return(0);
}

EntitySnapshot* ReplicationSystem::GetFullEntitySnapshot(int tick) {
	std::list<EntitySnapshot*>::iterator i = m_fullSnapshots.begin();
	for (i; i != m_fullSnapshots.end(); i++) {
		if ((*i)->tick == tick) {
			return(*i);
		}
	}

	return(0);
}

void ReplicationSystem::SendFullSnapshot(int sessionID) {
	m_sessionIDs.push_back(sessionID);
}