
#ifndef echorequestmessage_hpp
#define echorequestmessage_hpp

/**
 * Ping from client to server
 */
class EchoRequestMessage : public NetworkMessage {
public:
    EchoRequestMessage() = default;
    ~EchoRequestMessage() = default;
    
    /**
     * Send a connection request (client)
     */
    void OnSend();
    
    /**
     * Receive a connection request and reply (server)
     */
    void OnReceive();
};

#endif
