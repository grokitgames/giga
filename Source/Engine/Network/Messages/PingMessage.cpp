
#include <giga-engine.h>

void PingMessage::Parse() {
    // Read out the start time of the server
    uint32_t createSeconds, createNsecond;
    
    ReadPayload(&createSeconds, sizeof(uint32_t));
    ReadPayload(&createNsecond, sizeof(uint32_t));
    
    timespec ts;
    ts.tv_sec = createSeconds;
    ts.tv_nsec = createNsecond;
    
    // Create a pong message to be sent back
    PongMessage* msg = new PongMessage();
    msg->SetStartTime(ts);
    
    NetworkServer* networkServer = GetSystem<NetworkServer>();
    networkServer->Write(m_envelope.session, msg);
}

void PingMessage::CreatePayload() {
    // Get the current time
    timespec currentTime;
    Timer::GetTimestamp(&currentTime);
    
    uint32_t currentSeconds = (uint32_t)currentTime.tv_sec;
    uint32_t currentNsecond = (uint32_t)currentTime.tv_nsec;
    
    // Pack the current time
    unsigned char* message = (unsigned char*)malloc(sizeof(uint32_t) * 2);
    
    int offset = 0;
    memcpy(message + offset, &currentSeconds, sizeof(uint32_t));
    
    offset += sizeof(uint32_t);
    memcpy(message + offset, &currentNsecond, sizeof(uint32_t));
    
    SetPayload(message, sizeof(uint32_t) * 2);
}
