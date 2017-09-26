
#ifndef command_hpp
#define command_hpp

enum {
	COMMAND_START = 1,
	COMMAND_END
};

class GIGA_API Command : public EventData {
public:
    Command();
    ~Command() = default;
    
    GIGA_CLASS_NAME("Command");

	/**
	 * Initialize a new command
	 */
	void Initialize(Variant** argv, int argc);

	/**
	 * Record start/end
	 */
	void Start();
	void End();

	/**
	 * Register a new command type
	 */
	static void RegisterCommandType(std::string name, int typeID);

	/**
	 * Get current command ID
	 */
	static int GetLastCommandID() { return m_lastCommandID; }

	/**
	 * Scripting integration
	 */
	static Variant* RegisterCommandType(Variant* object, int argc, Variant** argv);
	static Variant* Start(Variant* object, int argc, Variant** argv);
	static Variant* End(Variant* object, int argc, Variant** argv);
	static Variant* Type(std::string prop, Variant* obj);
    
public:
	// The integer type (from the registration for serialization)
	uint32_t type;

	// The entity this command is occuring on
	uint32_t entityID;

	// The start time
    uint32_t start;

	// The end time
	uint32_t end;

	// The session ID
	uint32_t sessionID;

	// The command ID
	uint32_t commandID;

protected:
	struct CommandType {
		std::string name;
		uint32_t typeID;
	};

	static std::vector<CommandType*> m_commandTypes;

	static int m_lastCommandID;
};

#endif
