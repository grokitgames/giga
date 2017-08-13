
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
    static Variant* GetTimestamp(std::string var, Variant* object);
    static Variant* GetYear(std::string var, Variant* object);
    static Variant* GetMonth(std::string var, Variant* object);
    static Variant* GetDay(std::string var, Variant* object);
    static Variant* GetHour(std::string var, Variant* object);
    static Variant* GetMinute(std::string var, Variant* object);
    static Variant* GetSecond(std::string var, Variant* object);
    
private:
    // Use time_t struct for more general date/time storage
    time_t m_time;
};

#endif
