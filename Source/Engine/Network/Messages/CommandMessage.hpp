
/**
* Command from client to server
*/
class GIGA_API CommandMessage : public NetworkMessage, public ScriptableObject {
public:
	CommandMessage();
	~CommandMessage() = default;

	GIGA_CLASS_NAME("CommandMessage");

    /**
     * Set command
     */
    void SetCommand(Command* command) { m_command = command; }
    
	/**
	* Send a connection request (client)
	*/
	void OnSend();

	/**
	* Receive a connection request and reply (server)
	*/
	void OnReceive();

	/**
	 * Scripting integration
	 */
	void Initialize(Variant** argv, int argc);

protected:
	Command* m_command;
};
