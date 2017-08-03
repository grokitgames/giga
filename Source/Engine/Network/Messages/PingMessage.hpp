
#ifndef pingmessage_hpp
#define PingMessage_hpp

class GIGA_API PingMessage : public NetworkMessage {
public:
    PingMessage() = default;
    ~PingMessage() = default;
    
    void CreatePayload();
    void Parse();
};

#endif
