
#include <giga-engine.h>

NetworkMessage::NetworkMessage() {
    memset(&m_envelope, 0, sizeof(NetworkEnvelope));
    m_payload = 0;
	m_payloadOffset = 0;
}

NetworkMessage::~NetworkMessage() {
    if(m_payload) {
        free(m_payload);
        m_payload = 0;
    }
}

void NetworkMessage::Initialize(unsigned char* buffer, int size) {
    // Read in the message type (first thing in envelope)
    int offset = 0;
    
    memcpy(&m_envelope.type, buffer + offset, sizeof(int16_t));
    offset += sizeof(int16_t);
    
    // Session
    memcpy(&m_envelope.session, buffer + offset, sizeof(int32_t));
    offset += sizeof(int32_t);
    
    // Tick
    memcpy(&m_envelope.tick, buffer + offset, sizeof(uint32_t));
    offset += sizeof(uint32_t);
    
    // MSG ID
    memcpy(&m_envelope.id, buffer + offset, sizeof(uint32_t));
    offset += sizeof(uint32_t);
    
    // Ack
    memcpy(&m_envelope.flags, buffer + offset, sizeof(uint16_t));
    offset += sizeof(uint16_t);
    
    // Bytes
    memcpy(&m_envelope.bytes, buffer + offset, sizeof(int32_t));
    offset += sizeof(int32_t);
    
    // Chunk ID
    memcpy(&m_envelope.chunkID, buffer + offset, sizeof(uint16_t));
    offset += sizeof(uint16_t);
    
    // End
    memcpy(&m_envelope.end, buffer + offset, sizeof(uint16_t));
    offset += sizeof(uint16_t);
    
    // Otherwise, we have a message to process, read in the rest
    m_payload = (unsigned char*)malloc(m_envelope.bytes);
    memcpy(m_payload, buffer + offset, m_envelope.bytes);
}

void NetworkMessage::Append(unsigned char* data, int start, int size) {
    GIGA_ASSERT(start + size < m_envelope.bytes, "Buffer overflow.");
    memcpy(m_payload + start, data, size);
}

unsigned char* NetworkMessage::GetPayload(int &size) {
    size = sizeof(NetworkEnvelope) + m_envelope.bytes;
    unsigned char* buffer = (unsigned char*)malloc(size);
    
    // Write the message type (first thing in envelope)
    int offset = 0;
    
    memcpy(buffer + offset, &m_envelope.type, sizeof(int16_t));
    offset += sizeof(int16_t);
    
    // Session
    memcpy(buffer + offset, &m_envelope.session, sizeof(int32_t));
    offset += sizeof(int32_t);
    
    // Tick
    memcpy(buffer + offset, &m_envelope.tick, sizeof(uint32_t));
    offset += sizeof(uint32_t);
    
    // MSG ID
    memcpy(buffer + offset, &m_envelope.id, sizeof(int32_t));
    offset += sizeof(int32_t);
    
    // Ack
    memcpy(buffer + offset, &m_envelope.flags, sizeof(int16_t));
    offset += sizeof(int16_t);
    
    // Bytes
    memcpy(buffer + offset, &m_envelope.bytes, sizeof(int32_t));
    offset += sizeof(int32_t);
    
    // Chunk ID
    memcpy(buffer + offset, &m_envelope.chunkID, sizeof(uint16_t));
    offset += sizeof(uint16_t);
    
    // End
    memcpy(buffer + offset, &m_envelope.end, sizeof(uint16_t));
    offset += sizeof(uint16_t);
    
    // Otherwise, we have a message to process, read in the rest
    memcpy(buffer + offset, m_payload, m_envelope.bytes);
    
    return(buffer);
}

void NetworkMessage::ReadPayload(void* bytes, int size) {
	// printf("Bytes: %d, offset: %d, reading: %d\n", m_envelope.bytes, m_payloadOffset, size);
    GIGA_ASSERT(m_payloadOffset + size <= m_envelope.bytes, "Cannot read more than size of packet.");
    
    memcpy(bytes, m_payload + m_payloadOffset, size);
    m_payloadOffset += size;
}

void NetworkMessage::SetPayload(unsigned char* bytes, int size) {
    m_payload = bytes;
    m_envelope.bytes = size;
}
