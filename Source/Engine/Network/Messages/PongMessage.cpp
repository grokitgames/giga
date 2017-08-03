
#include <giga-engine.h>

void PongMessage::Parse() {
    // Read out the start time of the server
    uint32_t startSeconds, startNsecond;
    uint32_t currentSeconds, currentNsecond;
    uint32_t createSeconds, createNsecond;
    
    ReadPayload(&startSeconds, sizeof(uint32_t));
    ReadPayload(&startNsecond, sizeof(uint32_t));
    ReadPayload(&currentSeconds, sizeof(uint32_t));
    ReadPayload(&currentNsecond, sizeof(uint32_t));
    ReadPayload(&createSeconds, sizeof(uint32_t));
    ReadPayload(&createNsecond, sizeof(uint32_t));
    
    // Get our current UTC timestamp
    timespec current;
    Timer::GetTimestamp(&current);
    
    // Compute difference from local UTC timestamp to server
    double currentTime = (double)current.tv_sec + ((double)current.tv_nsec / 1000000000);
    double currentServerTime = (double)currentSeconds + ((double)currentNsecond / 1000000000);
    double createTime =(double)createSeconds + ((double)createNsecond / 1000000000);
    
    double lag = (currentTime - createTime) / 2.0f;
    double difference = currentServerTime - lag - currentTime;
    
    timespec startTime;
    startTime.tv_sec = startSeconds;
    startTime.tv_nsec = startNsecond;
    
    // Set the start time of our network system
    NetworkClient* networkSystem = GetSystem<NetworkClient>();
    
    // Get connection info
    NetworkConnectionInfo info = networkSystem->GetConnectionInfo();
    info.serverStartTime = startTime;
    info.clientTimeDiff = difference;
}

void PongMessage::CreatePayload() {
    // Get our server from the application
    NetworkServer* networkServer = GetSystem<NetworkServer>();
    
    // Get the start time of the server
    timespec startTime;
    networkServer->GetStartTime(startTime);
    
    uint32_t startSeconds = startTime.tv_sec;
    uint32_t startNsecond = startTime.tv_nsec;
    
    // Get the current time of the server
    timespec currentTime;
    Timer::GetTimestamp(&currentTime);
    
    uint32_t currentSeconds = currentTime.tv_sec;
    uint32_t currentNsecond = currentTime.tv_nsec;
    
    // Send back the original "ping" send time
    uint32_t pingSeconds = m_startTime.tv_sec;
    uint32_t pingNsecond = m_startTime.tv_nsec;
    
    // Pack the current time + start time + current tick
    unsigned char* message = (unsigned char*)malloc(sizeof(uint32_t) * 6);
    
    int offset = 0;
    memcpy(message + offset, &startSeconds, sizeof(uint32_t));
    
    offset += sizeof(uint32_t);
    memcpy(message + offset, &startNsecond, sizeof(uint32_t));
    
    offset += sizeof(uint32_t);
    memcpy(message + offset, &currentSeconds, sizeof(uint32_t));
    
    offset += sizeof(uint32_t);
    memcpy(message + offset, &currentNsecond, sizeof(uint32_t));
    
    offset += sizeof(uint32_t);
    memcpy(message + offset, &pingSeconds, sizeof(uint32_t));
    
    offset += sizeof(uint32_t);
    memcpy(message + offset, &pingNsecond, sizeof(uint32_t));
    
    SetPayload(message, sizeof(uint32_t) * 6);
}
