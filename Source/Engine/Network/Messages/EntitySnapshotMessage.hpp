
#ifndef entitysnapshotmessage_hpp
#define entitysnapshotmessage_hpp

/**
 * Receive the state of entities that have component-level changes with the server
 */
class GIGA_API EntitySnapshotMessage : public NetworkMessage {
public:
    EntitySnapshotMessage() = default;
    ~EntitySnapshotMessage() = default;
    
    /**
     * Send current entity state (from server)
     */
    void OnSend();
    
    /**
     * Receive entity state (client)
     */
    void OnReceive();
    
    /**
     * Set entity payload
     */
    void SetEntityPayload(unsigned char* payload, int size);
};

#endif
