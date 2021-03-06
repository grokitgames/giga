
#ifndef entitysnapshot_hpp
#define entitysnapshot_hpp

/**
 * A snapshot of data changes that have occured on entities in a specific tick
 */
class GIGA_API EntitySnapshot {
public:
    EntitySnapshot() = default;
    ~EntitySnapshot();
    
    enum {
        SNAPSHOT_FULL = 1,
        SNAPSHOT_DELTA,
    };
    
    /**
     * Serialize entities and components into a byte stream, providing a buffer of bufferSize
     * Starting from offset entities in, updated to reflect current offset at completion
     */
    void Serialize(unsigned char* buffer, int& bufferSize, int& offset);
    
    /**
     * Deserialize message
     */
    void Deserialize(unsigned char* buffer, int bufferSize);
    
public:
    // List of entities
    std::vector<Entity*> entities;
    
    // Snapshot time (in ticks)
    int tick;
    
    // Snapshot type
    int type;
};

#endif
