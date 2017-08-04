
#include <giga-engine.h>

bool PosixUDPSocket::Connect(std::string server, int port) {
    m_server = server;
    m_port = port;
    
    // Create a socket
    m_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if(m_socket < 0) {
        ErrorSystem::Process(new Error(ERROR_WARN, "Unable to create socket for server", server));
        return(false);
    }
    
    // Get the host IP in a computer usable home
    struct hostent* host;
    host = gethostbyname(server.c_str());
    if(host == 0) {
        ErrorSystem::Process(new Error(ERROR_WARN, "Unable to get host", server));
        return(false);
    }
    
    int flags = fcntl(m_socket, F_GETFL, 0);
    if (flags < 0) return false;
    flags = flags|O_NONBLOCK;
    if(fcntl(m_socket, F_SETFL, flags) != 0) {
        ErrorSystem::Process(new Error(ERROR_WARN, "Unable to set non-blocking connection type.", server));
        return(false);
    }
    
    // Create our server structure
    m_sockaddr.sin_family = AF_INET;
    bcopy((char *)host->h_addr, (char *)&m_sockaddr.sin_addr.s_addr, host->h_length);
    m_sockaddr.sin_port = htons(port);
    
    // Connect
    if(::connect(m_socket, (struct sockaddr*)&m_sockaddr, sizeof(m_sockaddr)) < 0) {
        ErrorSystem::Process(new Error(ERROR_WARN, "Unable to connect to host", server));
        return(false);
    }
    
    m_addrlen = sizeof(m_sockaddr);
    m_connected = true;
    
    return(true);
}

int PosixUDPSocket::Write(unsigned char* buffer, int length) {
    socklen_t len = sizeof(m_sockaddr);
    int ret = 0;
    
    if(m_connected) {
        ret = (int)send(m_socket, (void*)buffer, length, 0);
    }
    else {
        ret = (int)sendto(m_socket, (void*)buffer, length, 0, (struct sockaddr*)&m_sockaddr, len);
    }
    
    return(ret);
}

int PosixUDPSocket::Read(void* buffer, int length) {
    int ret = 0;
    socklen_t len = sizeof(sockaddr_in);
    
    if(m_connected) {
        ret = (int)recv(m_socket, buffer, length, MSG_DONTWAIT);
    }
    else {
        ret = (int)recvfrom(m_socket, buffer, length, MSG_DONTWAIT, (struct sockaddr*)&m_lastsockaddr, &len);
        m_lastLength = len;
    }
    
    return(ret);
}

void PosixUDPSocket::Close() {
    if(m_socket) {
        close(m_socket);
        m_socket = 0;
    }
}

void PosixUDPSocket::Listen(int port) {
    m_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if(m_socket < 0) {
        ErrorSystem::Process(new Error(ERROR_WARN, "Unable to initialize socket."));
        return;
    }
    
    struct sockaddr_in servaddr;
    memset(&servaddr, 0, sizeof(sockaddr_in));
    
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(port);
    servaddr.sin_addr.s_addr = INADDR_ANY;
    
    if(bind(m_socket, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        ErrorSystem::Process(new Error(ERROR_WARN, "Unable to bind socket."));
        return;
    }
}

void PosixUDPSocket::Accept(int socket) {
    m_socket = socket;
}

void PosixUDPSocket::SetSocketAddress(sockaddr_in* sock, socklen_t length) {
    m_sockaddr.sin_family = sock->sin_family;
    memcpy((void *)&m_sockaddr.sin_addr, (void*)&sock->sin_addr, sock->sin_len);
    m_sockaddr.sin_port = sock->sin_port;
}

sockaddr_in* PosixUDPSocket::GetLastSocketAddress(socklen_t& len) {
    len = m_lastLength;
    return(&m_lastsockaddr);
}
