
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
    unsigned char* message = (unsigned char*)malloc(sizeof(uint32_t) * 3);
    
    int offset = 0;
    memcpy(message + offset, &currentSeconds, sizeof(uint32_t));
    
    offset += sizeof(uint32_t);
    memcpy(message + offset, &currentNsecond, sizeof(uint32_t));

	NetworkSystem* networkSystem = GetSystem<NetworkSystem>();
	NetworkSession* session = networkSystem->FindSession(0);
	offset += sizeof(uint32_t);

	uint32_t pingTime = floor(session->info.pingTime * 1000);
	memcpy(message + offset, &pingTime, sizeof(uint32_t));
    
	offset += sizeof(uint32_t);
	uint32_t offsetTime = floor(session->info.clientTimeDiff * 1000);
	memcpy(message + offset, &offsetTime, sizeof(uint32_t));

    SetPayload(message, sizeof(uint32_t) * 4);
}

void EchoRequestMessage::OnReceive() {
    // Receive the local timestamp from a client and echo it back along with our server start time
    
    // Read out the current timestamp of the client
    uint32_t clientSeconds, clientNsecond;
	uint32_t clientPing, clientOffset;
    
    ReadPayload(&clientSeconds, sizeof(uint32_t));
    ReadPayload(&clientNsecond, sizeof(uint32_t));
	ReadPayload(&clientPing, sizeof(uint32_t));
	ReadPayload(&clientOffset, sizeof(uint32_t));
    
    timespec ts;
    ts.tv_sec = clientSeconds;
    ts.tv_nsec = clientNsecond;
    
    // Create a reply
    EchoResponseMessage* response = new EchoResponseMessage();
    response->SetClientTimestamp(ts);
    
    // Server will send this back
    NetworkSystem* networkSystem = GetSystem<NetworkSystem>();
    networkSystem->Send(response);
    
    // Update last ping time for session
    NetworkSession* session = networkSystem->FindSession(m_envelope.session);
    session->lastPing = networkSystem->GetCurrentTick();
	session->info.pingTime = (float)clientPing / 1000.0f;
	session->info.clientTimeDiff = (float)clientOffset / 1000.0f;

	printf("Client ping time: %d ms, offset %d ms\n", clientPing, clientOffset);
}
