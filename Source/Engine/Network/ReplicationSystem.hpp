
#ifndef replicationsystem_hpp
#define replicationsystem_hpp

// The number of seconds to maintain snapshot history for
#define NETWORK_SNAPSHOT_HISTORY				NETWORK_TICKS_PER_SECOND * 3
#define NETWORK_SNAPSHOT_RENDER_LAG				2
#define NETWORK_REPLICATION_CORRECTION_TIME		5

enum {
    REPLICATION_SERVER = 1,
    REPLICATION_CLIENT
};

/**
 * Network system responsible for maintaining past, current and future snapshots
 */
class GIGA_API ReplicationSystem : public System {
public:
    ReplicationSystem();
    ~ReplicationSystem();
    
    GIGA_CLASS_NAME("ReplicationSystem");
    
	/**
	* Overridable initialization function
	*/
	void Initialize();

    /**
     * Update (remove old snapshots and create current)
     */
    void Update(float delta);
    
    /**
     * Set type
     */
    void SetType(int type) { m_type = type; }

	/**
	 * Add snapshot to stack
	 */
	void AddSnapshot(int tick, EntitySnapshot* snapshot);
	void AddFullSnapshot(int tick, EntitySnapshot* snapshot);

	/**
	 * Get previous snapshot
	 */
	EntitySnapshot* GetEntitySnapshot(int tick);
	EntitySnapshot* GetFullEntitySnapshot(int tick);

	/**
	 * Queue up a session to receive a full snapshot
	 */
	void SendFullSnapshot(int sessionID);

	/**
	 * Add command to history
	 */
	void AddCommand(Command* command);

	/**
	 * Start command
	 */
	void StartCommand(Command* command);
	void EndCommand(Command* command);

	/**
	 * Set command as received
	 */
	void SetClientAuthoritative(bool authoritative);

	/**
	 * Find a command
	 */
	Command* GetCommand(int entityID, int commandID);
    
protected:
    /**
     * Internal application of snapshot
     */
    void ApplySnapshot(EntitySnapshot* current, EntitySnapshot* next, float interpolate);

	/**
	 * Event handler for command end
	 */
	static void CommandStartHandler(Event* event);
	static void CommandEndHandler(Event* event);
    
protected:
    // Historical (+ potential future) snapshots
    std::vector<EntitySnapshot*> m_snapshots;

	// Full historical snapshots
	std::vector<EntitySnapshot*> m_fullSnapshots;

	// Command history
	struct CommandTick {
		std::vector<Command*> commands;
	};

	std::map<int, CommandTick*> m_commandHistory;
    
    // Currently open commands
    std::vector<Command*> m_activeCommands;
	
	// The tick we need to roll back to for any new commands
	int m_commandTick;

    // Last processed server tick
    int m_lastTick;
    
    // Type
    int m_type;
    
    // Whether we have applied a full snapshot yet
    bool m_initialized;

	// Sessions that we need to send a full snapshot to next tick
	std::vector<int> m_sessionIDs;

	// Whether the replication system is currently in "replay" mode
	bool m_replay;

	// Whether the client is authoritative for the player entity for the time being
	bool m_clientAuthoritative;
	bool m_clientShouldBeAuthoritative;
	bool m_checkPredictionErrors;

	// A list of on-going prediction error corrections
	std::vector<ClientPredictionError*> m_predictionErrors;
};

#endif
