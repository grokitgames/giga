
#ifndef networksystem_hpp
#define networksystem_hpp

#define NETWORK_MAX_PACKET_SIZE     8000
#define NETWORK_TICKS_PER_SECOND    20
#define NETWORK_ECHO_TICKS          10
#define NETWORK_ECHO_TIMEOUT        60
#define NETWORK_RESEND_MULTIPLE     2
#define NETWORK_RESEND_HISTORY      60

/**
 * Info specific to acting as a client
 */
struct GIGA_API NetworkClientInfo {
    // Time offset to server
    timespec timeOffset;
    
    // Our socket descriptor to check on
    fd_set sockets;
    
    // Our network session with the server
    NetworkSession* session;

	// Session ID (set before session object is created)
	int sessionID;

	// Player entity ID
	int playerID;
};

/**
 * Info specific to acting as a server
 */
struct GIGA_API NetworkServerInfo {
    // Our set of sockets to check
    fd_set listenSocket;
    
    // Listening socket (server)
    UDPSocket* socket;
    
    // Networking sessions tracked by server
    std::vector<NetworkSession*> sessions;
};

union NetworkSystemInfo {
    NetworkClientInfo* client_info;
    NetworkServerInfo* server_info;
};

/**
 * Partial messages
 */
class GIGA_API NetworkMessagePart {
public:
    NetworkMessagePart();
    ~NetworkMessagePart();
    
public:
    // Master message; all concatenated parts
    NetworkMessage* master;
    
    // Parts of the message (for tracking)
    std::list<NetworkMessage*> parts;
    
    // Last tick we received message in
    uint32_t lastTick;
    
    // Network session this is attached to
    NetworkSession* session;
    
    // Whether this message has already been processed (in case of receiving partials twice)
    bool processed;
};

/**
 * Network system, handling sessions - can act as client or server
 */
class GIGA_API NetworkSystem : public System {
public:
    NetworkSystem();
    ~NetworkSystem();
    
    GIGA_CLASS_NAME("NetworkSystem");
    
    enum {
        NETWORK_SYSTEM_CLIENT = 1,
        NETWORK_SYSTEM_SERVER
    };
    
    /**
     * Initialize (set startup time)
     */
    void Initialize();
    
    /**
     * Connect to remote server (when connected as client)
     */
    void Connect(std::string host, int port);
    
    /**
     * Listen for new connections on a particular port (server)
     */
    void Listen(int port);
    
    /**
     * Send a message to all connected sessions
     */
    void Send(NetworkMessage* msg);
    
    /**
     * Send a message to a single session
     */
    void Send(int sessionID, NetworkMessage* msg);
    
    /**
     * Process/receive new messages
     */
    void Update(float delta);
    
    /**
     * Get current tick
     */
    int GetCurrentTick();
    
    /**
     * Get current offset time since start
     */
    float GetCurrentTime();
    
    /**
     * Get/set server startup time
     */
    timespec GetStartupTime() { return m_startupTime; }
    void SetStartupTime(timespec t) { m_startupTime = t; }
    
    /**
     * Add a session to our tracked sessions (server)
     */
    NetworkSession* FindSession(int sessionID, UDPSocket* socket = 0);
    
    /**
     * Remove a session
     */
    void RemoveSession(int sessionID);

	/**
	 * Set a manual "tick offset" for replaying
	 */
	void SetTick(int tick) { m_overrideTick = tick; }
    
    /**
     * Register a new message type (by class and unique ID)
     */
    template<typename T>
    void RegisterMessageType(int messageID) {
        // Make sure this type isn't already registered
        std::map<int, NetworkMessage*(*)()>::iterator i = m_messageTypes.find(messageID);
        GIGA_ASSERT(i == m_messageTypes.end(), "Message type ID already registered.");
        
        // Register it (by message ID for faster lookup on incoming packets)
        m_messageTypes[messageID] = &CreateMessage<T>;
    }

	/**
	 * Mark a message as received (ack)
	 */
	void MarkReceived(int sessionID, int messageID);

	/**
	 * Get network system info
	 */
	NetworkSystemInfo* GetSystemInfo() { return &m_info; }

	/**
	 * Scripting integration
	 */
	static Variant* Send(Variant* object, int argc, Variant** argv);
    
protected:
	// Internal send function
	void Send(NetworkSession* session, NetworkMessage* msg);

protected:
    // Create a new message from a class type
    template<typename T> static NetworkMessage* CreateMessage() { return new T; }
    
protected:
    // Max socket ID we should poll for
    int m_maxSocketID;
    
    // Startup time
    timespec m_startupTime;
    
    // Mapping of registered network message types
    std::map<int, NetworkMessage*(*)()> m_messageTypes;
    
    // Client/server specific information
    NetworkSystemInfo m_info;
    
    // Setup type
    int m_systemType;
    
    // Last message ID sent
    int m_lastMessageID;
    
    // Map of message IDs to incomplete message parts
    std::map<uint32_t, NetworkMessagePart*> m_partials;

	// Manual tick offset for replaying
	int m_overrideTick;

	// Network messages waiting for an ack response
	std::list<NetworkMessage*> m_acks;
};

#endif
