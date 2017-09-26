
#include <giga-engine.h>

void EntitySnapshotMessage::SetEntityPayload(unsigned char* payload, int size) {
    SetPayload(payload, size);
}

void EntitySnapshotMessage::OnSend() {
    // Nothing to put here, payload set by SetEntityPayload
	m_envelope.type = 30;
}

void EntitySnapshotMessage::OnReceive() {
	EntitySnapshot* snapshot = new EntitySnapshot();
	snapshot->tick = m_envelope.tick;
	snapshot->Deserialize(m_payload, m_envelope.bytes);

	ReplicationSystem* replicationSystem = GetSystem<ReplicationSystem>();
	replicationSystem->AddSnapshot(m_envelope.tick, snapshot);

	// Check if we can make the server authoritative again
	if (m_envelope.lastCmd >= Command::GetLastCommandID()) {
		replicationSystem->SetClientAuthoritative(false);
	}
}
