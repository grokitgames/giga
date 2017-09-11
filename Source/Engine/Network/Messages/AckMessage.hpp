
#ifndef ackmessage_hpp
#define ackmessage_hpp

/**
* Ack respone message
*/
class GIGA_API AckMessage : public NetworkMessage {
public:
	AckMessage() = default;
	~AckMessage() = default;

	/**
	* Send a connection request (client)
	*/
	void OnSend();

	/**
	* Receive a connection request and reply (server)
	*/
	void OnReceive();

public:
	// Message ID
	uint32_t messageID;
};

#endif
