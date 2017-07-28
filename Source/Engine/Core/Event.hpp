
#ifndef event_hpp
#define event_hpp

/**
 * Generic event container - will contain event-specific EventData
 */
class GIGA_API Event : public ScriptableObject {
public:
    Event() : m_type(0) { }
    Event(std::string type) : m_type(type), m_data(0), m_sender(0) { }
    Event(std::string type, EventData* data) : m_type(type), m_data(data), m_sender(0) { }
    Event(std::string type, EventData* data, GigaObject* sender) : m_type(type), m_data(data), m_sender(sender) { }
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
    GigaObject* GetSender() { return m_sender; }
    
    /**
     * Scripting integration
     */
    static Variant* GetType(Variant* object);
    static Variant* GetData(Variant* object);
    static Variant* GetSender(Variant* object);
    
protected:
    // Event type
    std::string m_type;
    EventData* m_data;
    GigaObject* m_sender;
};

#endif
