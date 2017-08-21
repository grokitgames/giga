
#include <giga-engine.h>

NetworkSystem::NetworkSystem() {
    m_systemType = 0;
    m_lastMessageID = 0;
	memset(&m_startupTime, 0, sizeof(timespec));
}

NetworkSystem::~NetworkSystem() {
    
}

void NetworkSystem::Initialize() {
    Timer::GetTimestamp(&m_startupTime);
}

void NetworkSystem::Connect(std::string host, int port) {
    // Set as client type
    m_systemType = NETWORK_SYSTEM_CLIENT;
    
    // Setup client info
    NetworkClientInfo* info = new NetworkClientInfo();
    info->timeOffset = {0, 0};
    FD_ZERO(&info->sockets);
    
    // Create a new network session
    NetworkSession* session = new NetworkSession();
    
    // Initialize player and session ID to null to start
    session->player = 0;
	session->sessionID = 0;
    
    // Create socket
    session->socket = new UDPSocket();
    if(session->socket->Connect(host, port) == false) {
        delete session;
        delete info;
        
        return;
    }
    
    // Save session info
    info->session = session;
    
    // Save
    m_info.client_info = info;
    
    // Set max socket ID for polling
    m_maxSocketID = std::max(m_maxSocketID, session->socket->GetSocket());
    FD_SET(session->socket->GetSocket(), &info->sockets);
    
    // Send first echo message request
    EchoRequestMessage* request = new EchoRequestMessage();
    session->Write(request);
}

void NetworkSystem::Listen(int port) {
    // Set as server type
    m_systemType = NETWORK_SYSTEM_SERVER;
    
    // Set up server info
    NetworkServerInfo* info = new NetworkServerInfo();
    FD_ZERO(&info->listenSocket);
    
    // Create a new socket and listen on it
    info->socket = new UDPSocket();
    info->socket->Listen(port);
    
    // Initialize our socket listener lists
    FD_SET(info->socket->GetSocket(), &info->listenSocket);
    
    // Save
    m_info.server_info = info;
    
    // Set max socket ID for polling
    m_maxSocketID = std::max(m_maxSocketID, info->socket->GetSocket());
}

void NetworkSystem::Send(NetworkMessage* msg) {
    // Fill in message ID and tick
    NetworkMessage::NetworkEnvelope* env = msg->GetEnvelope();
    env->tick = GetCurrentTick();
    env->id = ++m_lastMessageID;
    
    // Send to all connected sessions
    if(m_systemType == NETWORK_SYSTEM_CLIENT) {
        m_info.client_info->session->Write(msg);
    }
    else {
        for(size_t i = 0; i < m_info.server_info->sessions.size(); i++) {
            m_info.server_info->sessions[i]->Write(msg);
        }
    }
}

void NetworkSystem::Send(int sessionID, NetworkMessage* msg) {
    GIGA_ASSERT(m_systemType == NETWORK_SYSTEM_SERVER, "This function should only be called on servers.")

    // Fill in message ID and tick
    NetworkMessage::NetworkEnvelope* env = msg->GetEnvelope();
    env->tick = GetCurrentTick();
    env->id = ++m_lastMessageID;
    
    for(size_t i = 0; i < m_info.server_info->sessions.size(); i++) {
        if(m_info.server_info->sessions[i]->sessionID == sessionID) {
            m_info.server_info->sessions[i]->Write(msg);
            return;
        }
    }
    
    GIGA_ASSERT(false, "Session not found to send to.");
}

NetworkSession* NetworkSystem::FindSession(int sessionID, UDPSocket* socket) {
    // If this is the client, just return our default session
    if(m_systemType == NETWORK_SYSTEM_CLIENT) {
        return(m_info.client_info->session);
    }
    
    // Otherwise, try to find our session
    for(size_t i = 0; i < m_info.server_info->sessions.size(); i++) {
        if(m_info.server_info->sessions[i]->sessionID == sessionID) {
            return(m_info.server_info->sessions[i]);
        }
    }
    
    GIGA_ASSERT(socket != 0, "Session not found and socket data not supplied.");
    
    // Add a new session
    NetworkSession* session = new NetworkSession();
    
    // Set socket properties
    session->socket = new UDPSocket();
    
    // Get data for the last socket we read from
    socklen_t len = 0;
    sockaddr_in* in = socket->GetLastSocketAddress(len);
    
    // Set it on new socket for future sending
    session->socket->SetSocketAddress(in, len);
    session->sessionID = sessionID;
	session->socket->SetSocket(socket->GetSocket());
    
    // Add to list
    m_info.server_info->sessions.push_back(session);

	Application::Log(ERROR_DEBUG, "New session", std::to_string(sessionID));
    
    return(session);
}

void NetworkSystem::RemoveSession(int sessionID) {
    if(m_systemType == NETWORK_SYSTEM_CLIENT) {
        delete m_info.client_info->session;
        m_info.client_info->session = 0;
    }
    
    std::vector<NetworkSession*>::iterator i = m_info.server_info->sessions.begin();
    for(i; i != m_info.server_info->sessions.end(); i++) {
        if((*i)->sessionID == sessionID) {
            delete (*i);
            m_info.server_info->sessions.erase(i);
            
            return;
        }
    }
}

void NetworkSystem::Update(float delta) {
    // Make sure we're initialized and have something to update
    if(m_systemType == 0) {
        return;
    }
    
    // Get current tick
    int tick = GetCurrentTick();
    
    // Check if there is any data waiting
    timeval wait = {0, 0};
    
    fd_set readfds;
    UDPSocket* socket = 0;
    if(m_systemType == NETWORK_SYSTEM_CLIENT) {
        readfds = m_info.client_info->sockets;
        socket = m_info.client_info->session->socket;
    }
    else {
        readfds = m_info.server_info->listenSocket;
        socket = m_info.server_info->socket;
    }
    
    if(select(m_maxSocketID + 1, &readfds, 0, 0, &wait)) {
        // Create a buffer big enough to hold a large-ish packet
        unsigned char* buffer = (unsigned char*)malloc(NETWORK_MAX_PACKET_SIZE);
        int bytes = socket->Read(buffer, NETWORK_MAX_PACKET_SIZE);
        
        while(bytes > 0) {
            // Read in the message type (first thing in envelope)
            uint32_t type = 0;
            memcpy(&type, buffer, sizeof(uint32_t));
            
            NetworkMessage* msg = 0;
            std::map<int, NetworkMessage*(*)()>::iterator it = m_messageTypes.find((int)type);
            GIGA_ASSERT(it != m_messageTypes.end(), "Unregistered message type.");
            
            // Call the constructor for the specific class
            msg = it->second();
            msg->Initialize(buffer, bytes);
            
            // If we're in server mode, check to see if this is a new session
            NetworkMessage::NetworkEnvelope* env = msg->GetEnvelope();
            NetworkSession* session = FindSession(env->session, socket);
            
            // Parse the message
            msg->OnReceive();
            
            // Get the next message
            bytes = socket->Read(buffer, NETWORK_MAX_PACKET_SIZE);
        }
        
        if(bytes == 0) {
            // TODO: Socket closed, should be removed
        }
        
        // Free buffer data
        free(buffer);
    }
    
    // Send echo packet (if client)
    if(m_systemType == NETWORK_SYSTEM_CLIENT) {
        if(m_info.client_info->session->lastPing < (tick - NETWORK_ECHO_TICKS)) {
            EchoRequestMessage* request = new EchoRequestMessage();
            m_info.client_info->session->Write(request);
            
            m_info.client_info->session->lastPing = tick;
        }
    }
    
    if(m_systemType == NETWORK_SYSTEM_SERVER) {
		// Check for timed out clients
        for(size_t i = 0; i < m_info.server_info->sessions.size(); i++) {
            if(m_info.server_info->sessions[i]->lastPing < (tick - NETWORK_ECHO_TIMEOUT)) {
				printf("Last ping: %d, current tick: %d\n", m_info.server_info->sessions[i]->lastPing, tick);
				Application::Log(ERROR_DEBUG, "Disconnecting session", std::to_string(m_info.server_info->sessions[i]->sessionID));
                RemoveSession(m_info.server_info->sessions[i]->sessionID);
            }
        }
    }
}

int NetworkSystem::GetCurrentTick() {
    timespec t;
    Timer::GetTimestamp(&t);
    
    timespec diff = Timer::Diff(&m_startupTime, &t);
    double d = (double)diff.tv_sec + ((double)diff.tv_nsec / 1000000000);
    
    // If this is the client, add the offset in
    if(m_systemType == NETWORK_SYSTEM_CLIENT) {
        d += (double)m_info.client_info->timeOffset.tv_sec + ((double)m_info.client_info->timeOffset.tv_nsec / 1000000000);
    }
    
    return(floor(d * NETWORK_TICKS_PER_SECOND));
}
