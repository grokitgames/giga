
#ifndef resendrequestmessage_hpp
#define resendrequestmessage_hpp

class GIGA_API ResendRequestMessage : public NetworkMessage {
public:
    ResendRequestMessage();
    ~ResendRequestMessage();
    
    struct ResendRequestInfo {
        uint32_t messageID;
        uint32_t chunkID;
    };
    
    /**
     * Add missing packet details
     */
    void AddPacket(int messageID, int chunkID);
    
    /**
     * Compose message
     */
    void OnSend();
    
    /**
     * Process message
     */
    void OnReceive();
    
protected:
    // The packets and byte ranges we are re-requesting
    std::list<ResendRequestInfo*> m_packets;
};

#endif
