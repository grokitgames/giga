
#include <giga-engine.h>

void EchoRequestMessage::OnSend() {
    // Send the current timestamp of our client so we can receive it back in a connect reply and get RTT
	m_envelope.type = 10;
    
    // Get the current client time
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

void EchoRequestMessage::OnReceive() {
    // Receive the local timestamp from a client and echo it back along with our server start time
    
    // Read out the current timestamp of the client
    uint32_t clientSeconds, clientNsecond;
    
    ReadPayload(&clientSeconds, sizeof(uint32_t));
    ReadPayload(&clientNsecond, sizeof(uint32_t));
    
    timespec ts;
    ts.tv_sec = clientSeconds;
    ts.tv_nsec = clientNsecond;
    
    // Create a reply
    EchoResponseMessage* response = new EchoResponseMessage();
    response->SetClientTimestamp(ts);
    
    // Server will send this back
    NetworkSystem* networkSystem = GetSystem<NetworkSystem>();
    networkSystem->Send(response);
}
