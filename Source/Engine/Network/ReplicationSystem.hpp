
#ifndef replicationsystem_hpp
#define replicationsystem_hpp

// The number of seconds to maintain snapshot history for
#define NETWORK_SNAPSHOT_HISTORY    NETWORK_TICKS_PER_SECOND * 3
#define NETWORK_SNAPSHOT_RENDER_LAG 2

enum {
    REPLICATION_SERVER = 1,
    REPLICATION_CLIENT
};

/**
 * Network system responsible for maintaining past, current and future snapshots
 */
class GIGA_API ReplicationSystem : public System {
public:
    ReplicationSystem() : m_lastTick(0), m_type(0) { }
    ~ReplicationSystem();
    
    GIGA_CLASS_NAME("ReplicationSystem");
    
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
    
protected:
    // Historical (+ potential future) snapshots
    std::list<EntitySnapshot*> m_snapshots;
    
    // Last processed server tick
    int m_lastTick;
    
    // Type
    int m_type;
};

#endif
