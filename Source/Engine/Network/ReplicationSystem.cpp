
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
		// If this is a client, process any updates that need to happen
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

	// If we get here, we nede to put it at the front of the list
	m_snapshots.push_front(snapshot);
}
