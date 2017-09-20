
#include <giga-engine.h>

ReplicationSystem::ReplicationSystem() {
    m_lastTick = 0;
    m_type = 0;
    m_initialized = false;
	m_replay = false;
	m_commandTick = 0;
	m_clientAuthoritative = false;
	m_nextValidFrame = 0;
}

ReplicationSystem::~ReplicationSystem() {
    std::list<EntitySnapshot*>::iterator i = m_snapshots.begin();
    for(i; i != m_snapshots.end(); i++) {
        delete (*i);
    }
}

void ReplicationSystem::Initialize() {
	EventSystem* eventSystem = GetSystem<EventSystem>();
	eventSystem->RegisterEventHandler("COMMAND_END", &ReplicationSystem::CommandEndHandler);
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
			Entity* e1 = (*i)->Clone();

			// Add all components
			std::vector<Component*> components = (*i)->GetComponents();
			for (size_t j = 0; j < components.size(); j++) {
				Component* component = components[j]->Clone();
				e1->AddComponent(component);
			}
			fullSnapshot->entities.push_back(e1);

			// Then do delta snapshot
			if (true) {
				// Create a replica of the entity
				Entity* entity = 0;

				std::vector<Component*> components = (*i)->GetComponents();
				int updatedComponents = 0;
				for (size_t j = 0; j < components.size(); j++) {
					if (components[j]->HasUpdates()) {
						if (entity == 0) {
							entity = (*i)->Clone();
						}

						entity->AddComponent(components[j]->Clone());
						components[j]->MarkUpdated(false);
						updatedComponents++;
					}
				}

				// Add entity to snapshot
				if (entity) {
					snapshot->entities.push_back(entity);
				}
			}
		}
  
		// Save
		AddSnapshot(tick, snapshot);
		AddFullSnapshot(tick, fullSnapshot);

		if (m_replay == false) {
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

			// Send our full snapshot to any sessions that need it
			if (m_sessionIDs.size()) {
				EntitySnapshotMessage* msg = new EntitySnapshotMessage();
				unsigned char* packet = (unsigned char*)malloc(NETWORK_MAX_PACKET_SIZE);

				int size = NETWORK_MAX_PACKET_SIZE;
				int offset = 0;
				fullSnapshot->Serialize(packet, size, offset);

				msg->SetEntityPayload(packet, size);

				std::list<int>::iterator s = m_sessionIDs.begin();
				for (; s != m_sessionIDs.end(); s++) {
					networkSystem->Send(*s, msg);
				}

				delete msg;
				m_sessionIDs.clear();
			}

			// Do we have commands that need to be replayed? 
			if (m_commandTick > 0) {
				// Delete all entities
				printf("Command tick %d, deleting all entities.\n", m_commandTick);

				// Reset back to previous tick
				entitySystem->Clear();

				// Re-populate the entity system with the state as it existed in the full snapshot from that tick
				EntitySnapshot* snapshot = GetFullEntitySnapshot(m_commandTick);
				for (size_t i = 0; i < snapshot->entities.size(); i++) {
					Entity* entity = snapshot->entities[i]->Clone();

					std::vector<Component*> components = snapshot->entities[i]->GetComponents();
					for (size_t j = 0; j < components.size(); j++) {
						Component* component = components[j]->Clone();
						component->AddToSystem();

						entity->AddComponent(component);
					}

					entitySystem->AddEntity(entity);
				}

				// Loop over all ticks since then, replaying events
				m_replay = true;
				Application* application = Application::GetInstance();
				for (int t = m_commandTick; t <= tick; t++) {
					std::map<int, CommandTick*>::iterator ct = m_commandHistory.find(t);

					if (ct != m_commandHistory.end()) {
						std::list<Command*>::iterator c = m_commandHistory[t]->commands.begin();
						for (; c != m_commandHistory[t]->commands.end(); c++) {
							// Get our entity
							Entity* entity = entitySystem->FindEntity((*c)->entityID);
							GIGA_ASSERT(entity != 0, "Entity not found.");

							std::string eventStr = ((*c)->end > 0) ? "COMMAND_END" : "COMMAND_START";
							printf("Firing %s on entity %d.\n", eventStr.c_str(), (*c)->entityID);
							EventSystem::Process(new Event(eventStr, (*c), (*c)->entityID));
						}
					}

					networkSystem->SetTick(t);
					float newDelta = (1.0f / NETWORK_TICKS_PER_SECOND);

					printf("Current tick: %d, replaying tick %d with delta %f.\n", tick, t, newDelta);
					application->Update(newDelta);
					
					networkSystem->SetTick(0);
					tick = networkSystem->GetCurrentTick();
				}

				m_replay = false;
				m_commandTick = 0;
				networkSystem->SetTick(0);
				printf("Replay complete.\n");
			}

			// Clean up old command history
			std::map<int, CommandTick*>::iterator ct = m_commandHistory.begin();
			for (; ct != m_commandHistory.end(); ct++) {
				if ((*ct).first > (tick - NETWORK_SNAPSHOT_HISTORY)) {
					break;
				}

				std::list<Command*>::iterator c = (*ct).second->commands.begin();
				for (; c != (*ct).second->commands.end(); c++) {
					delete (*c);
				}
				(*ct).second->commands.clear();

				delete (*ct).second;
				m_commandHistory.erase((*ct).first);
			}
		}
	}
	else {
		// If this is a client, process any updates that need to happen

		// Adjust our tick by a set amount of "render lag" so that we can interpolate
		int renderTick = tick - NETWORK_SNAPSHOT_RENDER_LAG;
		if (renderTick > m_nextValidFrame) {
			m_clientAuthoritative = false;
		}

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

	if (m_replay == false) {
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
					if ((*i)->tick > (tick - NETWORK_SNAPSHOT_HISTORY))
						break;
					else 
						delete (*i);
				}

				m_fullSnapshots.erase(m_fullSnapshots.begin(), i);
			}
		}
	}

	// Save current tick as most recent processed
	m_lastTick = tick;

	PROFILE_END_AREA("ReplicationSystem Update");
}

void ReplicationSystem::ApplySnapshot(EntitySnapshot* current, EntitySnapshot* next, float interpolate) {
    // Get link to entity system
    EntitySystem* entitySystem = GetSystem<EntitySystem>();

	// Get our player entity ID
	NetworkSystem* networkSystem = GetSystem<NetworkSystem>();
	NetworkSession* session = networkSystem->FindSession(0);
	int playerID = session->playerID;

    // Apply changes from the startTick (now in i)
    for(size_t j = 0; j < current->entities.size(); j++) {
		if (m_clientAuthoritative && current->entities[j]->GetID() == playerID) {
			continue;
		}

        Entity* entity = entitySystem->FindEntity(current->entities[j]->GetID());
        if(entity == 0) {
            entity = new Entity();
            entity->SetID(current->entities[j]->GetID());
            entitySystem->AddEntity(entity);
        }
        
        std::vector<Component*> components = current->entities[j]->GetComponents();
        for(size_t k = 0; k < components.size(); k++) {
            Component* component = entity->FindComponent(components[k]->GetTypeID());
            if(component == 0) {
				component = components[k]->Clone();
				component->SetDataMappings();
				component->SetActive(true);
				component->AddToSystem();
            }
			else {
				components[k]->Copy(component);
			}
            
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
		for (int t = (*i)->tick; t < tick; t++) {
			EntitySnapshot* newSnapshot = new EntitySnapshot();
			newSnapshot->type = snapshot->type;
			newSnapshot->tick = t;

			for (size_t j = 0; j < snapshot->entities.size(); j++) {
				Entity* entity = new Entity();
				entity->SetID(snapshot->entities[j]->GetID());
				entity->SetName(snapshot->entities[j]->GetName());

				std::vector<Component*> components = snapshot->entities[j]->GetComponents();
				for (size_t k = 0; k < components.size(); k++) {
					Component* component = components[k]->Clone();
					entity->AddComponent(component);
				}

				newSnapshot->entities.push_back(entity);
			}

			m_snapshots.push_back(newSnapshot);
		}

		m_snapshots.push_back(snapshot);
		return;
	}

	// Otherwise, go through the list and find out where to insert
	for (i; i != m_snapshots.begin(); i--) {
        if(tick == (*i)->tick) {
            // If we get an exact match, replace
            delete (*i);
			auto p = m_snapshots.erase(i);
			m_snapshots.insert(p, snapshot);
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
		// If we're missing some snapshots in between, insert this one
		if ((*i)->tick < (tick - 1)) {
			for (int t = (*i)->tick; t < tick; t++) {
				EntitySnapshot* newSnapshot = new EntitySnapshot();
				newSnapshot->type = snapshot->type;
				newSnapshot->tick = t;
			
				for (size_t j = 0; j < snapshot->entities.size(); j++) {
					Entity* entity = new Entity();
					entity->SetID(snapshot->entities[j]->GetID());
					entity->SetName(snapshot->entities[j]->GetName());

					std::vector<Component*> components = snapshot->entities[j]->GetComponents();
					for (size_t k = 0; k < components.size(); k++) {
						Component* component = components[k]->Clone();
						entity->AddComponent(component);
					}

					newSnapshot->entities.push_back(entity);
				}

				m_fullSnapshots.push_back(newSnapshot);
			}
		}
		m_fullSnapshots.push_back(snapshot);
		return;
	}

	// Otherwise, go through the list and find out where to insert
	for (i; i != m_fullSnapshots.begin(); i--) {
		if (tick == (*i)->tick) {
			// If we get an exact match, replace
			delete (*i);
			auto p = m_fullSnapshots.erase(i);
			m_fullSnapshots.insert(p, snapshot);
			return;
		}

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

void ReplicationSystem::AddCommand(Command* command) {
	int tick = (command->end > 0) ? command->end : command->start;
	if (m_commandTick == 0) {
		m_commandTick = tick;
	}
	else {
		m_commandTick = std::min(m_commandTick, tick);
	}
	
	std::map<int, CommandTick*>::iterator i = m_commandHistory.find(tick);
	if (i == m_commandHistory.end()) {
		CommandTick* ct = new CommandTick();
		ct->commands.push_back(command);
		m_commandHistory[tick] = ct;
	}
	else {
		(*i).second->commands.push_back(command);
	}
}

Variant* ReplicationSystem::SendCommand(Variant* object, int argc, Variant** argv) {
    GIGA_ASSERT(argc == 1, "SendComand expects one argument.");
    GIGA_ASSERT(argv[0]->IsObject(), "First argument should be a command object.");
    
    ReplicationSystem* replicationSystem = GetSystem<ReplicationSystem>();
    replicationSystem->AddCommand(argv[0]->AsObject<Command>());
    
    CommandMessage* commandMessage = new CommandMessage();
    commandMessage->SetCommand(argv[0]->AsObject<Command>());
    
    NetworkSystem* networkSystem = GetSystem<NetworkSystem>();
    networkSystem->Send(commandMessage);
    
    delete commandMessage;
    return(new Variant(0));
}

void ReplicationSystem::CommandEndHandler(Event* event) {
	// When a command ends on the client, don't process any more snapshots until we receive another full snapshot
	ReplicationSystem* replicationSystem = GetSystem<ReplicationSystem>();
	if (replicationSystem->m_type == REPLICATION_SERVER) {
		return;
	}

	replicationSystem->m_clientAuthoritative = true;

	NetworkSystem* networkSystem = GetSystem<NetworkSystem>();
	NetworkSession* session = networkSystem->FindSession(0);

	// Get the command object
	std::list<EntitySnapshot*>::reverse_iterator i = replicationSystem->m_snapshots.rbegin();

	// Set the next valid received frame to the last tick received + 1.5 round-trip time (RTT)
	replicationSystem->m_nextValidFrame = (*i)->tick + (ceil(NETWORK_TICKS_PER_SECOND * session->info.pingTime) * 3);
}