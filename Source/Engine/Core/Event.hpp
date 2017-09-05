
#ifndef event_hpp
#define event_hpp

/**
 * Generic event container - will contain event-specific EventData
 */
class GIGA_API Event : public ScriptableObject {
public:
    Event() { }
    Event(std::string type) : m_type(type), m_data(0), m_entityID(0) { }
    Event(std::string type, EventData* data) : m_type(type), m_data(data), m_entityID(0) { }
    Event(std::string type, EventData* data, int sender) : m_type(type), m_data(data), m_entityID(sender) { }
    ~Event();

    GIGA_CLASS_NAME("Event");
    
    /**
     * Get/set type
     */
    std::string GetType() { return m_type; }
    
    /**
     * Get/set event-specific data
     */
    EventData* GetData() { return m_data; }
    
    /**
     * Get the originating object
     */
    int GetEntityID() { return m_entityID; }
    
    /**
     * Scripting integration
     */
    static Variant* GetType(std::string var, Variant* object);
    static Variant* GetData(std::string var, Variant* object);
    static Variant* GetEntityID(std::string var, Variant* object);
    
protected:
    // Event type
    std::string m_type;
    EventData* m_data;
    int m_entityID;
};

#endif
