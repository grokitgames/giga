
#ifndef timer_hpp
#define timer_hpp

class GIGA_API Timer : public ScriptableObject {
public:
    Timer();
    ~Timer();
    
    GIGA_CLASS_NAME("Timer");
    
    /**
     * Get current timestamp (UTC)
     */
    static void GetTimestamp(struct timespec *ts);
    
    /**
     * Get difference between two timespecs
     */
    static timespec Diff(timespec* start, timespec* end);
    
    /**
     * Start, get difference, reset and end timer
     */
    void Start();
    float Duration();
    float Reset();
    void End();
    
    /**
     * Sleep
     */
    static void Sleep(int milliseconds);
    
protected:
    // Start time of timer
    timespec m_startTime;
    timespec m_lastTime;
};

#endif
