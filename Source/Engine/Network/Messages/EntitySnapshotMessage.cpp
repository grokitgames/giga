
#include <giga-engine.h>

void EntitySnapshotMessage::SetEntityPayload(unsigned char* payload, int size) {
    SetPayload(payload, size);
}

void EntitySnapshotMessage::OnSend() {
    // Nothing to put here, payload set by SetEntityPayload
}

void EntitySnapshotMessage::OnReceive() {
    
}