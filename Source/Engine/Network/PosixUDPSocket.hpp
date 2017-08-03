
#ifndef posixudpsocket_hpp
#define posixudpsocket_hpp

class PosixUDPSocket : public UDPSocketBase {
public:
    PosixUDPSocket() = default;
    ~PosixUDPSocket() = default;
    
    /**
     * Connect to the server
     */
    bool Connect(std::string server, int port);
    
    /**
     * Send a message
     */
    int Write(unsigned char* buffer, int length);
    
    /**
     * Receive a message
     */
    int Read(void* buffer, int length);
    
    /**
     * Close the connection to the server
     */
    void Close();
    
    /**
     * Listen on a port for new "connections"
     */
    void Listen(int port);
    
    /**
     * "Accept" a new connection
     */
    void Accept(int socket);
    
    /**
     * Set the destination address
     */
    void SetSocketAddress(sockaddr_in* sock, socklen_t length);
    
    /**
     * Get the last known/read socket address
     */
    sockaddr_in* GetLastSocketAddress(socklen_t& len);
};

#endif
