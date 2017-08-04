
#ifndef echoresponsemessage_hpp
#define echoresponsemessage_hpp

/**
 * Server response to ping (syn) message
 */
class EchoResponseMessage : public NetworkMessage {
public:
    EchoResponseMessage() = default;
    ~EchoResponseMessage() = default;
    
    /**
     * Send a connection request (client)
     */
    void OnSend();
    
    /**
     * Receive a connection request and reply (server)
     */
    void OnReceive();
    
    /**
     * Set timestamp set by client
     */
    void SetClientTimestamp(timespec t) { m_clientTime = t; }
    
protected:
    // Client time (for calculating RTT)
    timespec m_clientTime;
};

#endif
