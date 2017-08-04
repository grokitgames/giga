
#ifndef networkmessage_hpp
#define networkmessage_hpp

/**
 * Base class for other message types
 */
class GIGA_API NetworkMessage {
public:
    NetworkMessage();
    ~NetworkMessage();
    
    /**
     * A network message header/envelope
     */
    struct NetworkEnvelope {
        uint32_t tick;
        uint32_t id;
        uint32_t type;
        uint32_t session;
        uint32_t bytes;
        uint32_t ack;
    };
    
    /**
     * Read a message in from a packet
     */
    void Initialize(unsigned char* buffer, int size);
    
    /**
     * Parse an existing message out of the payload (called on receive)
     */
    virtual void OnReceive() = 0;
    
    /**
     * Serialize a message into the payload (called on send)
     */
    virtual void OnSend() = 0;
    
    /**
     * Get a packet with envelope + payload to be sent
     */
    unsigned char* GetMessage(int& size);
    
    /**
     * Get our envelope
     */
    NetworkEnvelope* GetEnvelope() { return &m_envelope; }
    
    /**
     * Does this message require an ack?
     */
    void Ack(bool ack) { m_envelope.ack = ack; }
    
protected:
    /**
     * Helper function to go through payload, reading bytes (keeps track of offset)
     */
    void ReadPayload(void* bytes, int size);
    
    /**
     * Helper function to set payload (sets envelope size)
     */
    void SetPayload(unsigned char* bytes, int size);
    
protected:
    // The envelope/header
    NetworkEnvelope m_envelope;
    
    // The payload (data)
    unsigned char* m_payload;
    
private:
    // Current payload offset (for ReadPayload function)
    unsigned int m_payloadOffset;
};

#endif
