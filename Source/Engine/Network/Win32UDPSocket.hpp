
#ifndef win32udpsocket_hpp
#define win32udpsocket_hpp

class Win32UDPSocket : public UDPSocketBase {
public:
	Win32UDPSocket();
	~Win32UDPSocket();

public:
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
};

#endif