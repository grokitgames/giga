
#ifndef pongmessage_hpp
#define pongmessage_hpp

class GIGA_API PongMessage : public NetworkMessage {
public:
    PongMessage() = default;
    ~PongMessage() = default;
    
    // Set the time the "ping" message that spawned this was created
    void SetStartTime(timespec ts) { m_startTime = ts; }
    
    void CreatePayload();
    void Parse();
    
private:
    timespec m_startTime;
};

#endif
