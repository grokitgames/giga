
#ifndef datetime_hpp
#define datetime_hpp

class GIGA_API DateTime : public ScriptableObject {
public:
    DateTime() = default;
    ~DateTime() = default;
    
    GIGA_CLASS_NAME("DateTime");
    
    /**
     * Express time as string
     */
    std::string ToString();
    
    /**
     * Get current datetime
     */
    static DateTime* GetCurrent();
    
    /**
     * Get timestamp
     */
    uint64_t GetTimestamp();
    
    /**
     * Get pieces
     */
    int GetYear();
    int GetMonth();
    int GetDay();
    int GetHour();
    int GetMinute();
    int GetSecond();
    
    /**
     * Scripting integration
     */
    static Variant* GetCurrent(Variant* object, int argc, Variant** argv);
    static Variant* GetTimestamp(Variant* object);
    static Variant* GetYear(Variant* object);
    static Variant* GetMonth(Variant* object);
    static Variant* GetDay(Variant* object);
    static Variant* GetHour(Variant* object);
    static Variant* GetMinute(Variant* object);
    static Variant* GetSecond(Variant* object);
    
private:
    // Use time_t struct for more general date/time storage
    time_t m_time;
};

#endif
