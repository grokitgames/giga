
#ifndef profiler_hpp
#define profiler_hpp

#define PROFILE_START_FRAME() { Profiler::GetInstance()->StartFrame(); }
#define PROFILE_END_FRAME() { Profiler::GetInstance()->EndFrame(); }
#define PROFILE_START_AREA(x) { Profiler::GetInstance()->StartArea(x); }
#define PROFILE_END_AREA(x) { Profiler::GetInstance()->EndArea(x); }

class GIGA_API Profiler {
public:
    Profiler();
    ~Profiler();
    
    // Dump profiler times out to the application/debug file
    void Log();
    
    // Start a frame
    void StartFrame();
    
    // End the frame
    void EndFrame();
    
    // Start an area
    void StartArea(std::string name);
    
    // End an area
    void EndArea(std::string name);
    
    // Singleton
    static Profiler* GetInstance();
    
protected:
    // An area we are profiling
    class ProfileArea {
    public:
        ProfileArea();
        ~ProfileArea();
    public:
        // The name of the section/area we're profiling
        std::string areaName;
        std::map<std::string, ProfileArea*> children;
        
        // The average frame time so far while we've been running
        float avgFrameTime;
        
        // The current start time (as a delta to the frame start time)
        float currentStartTime;
        
        // Our parent profile area
        ProfileArea* parent;
    };
    
    void RecursiveLog(int level, Profiler::ProfileArea* area);
    
protected:
    
    ProfileArea* m_currentArea;
    ProfileArea* m_rootArea;
    Timer* m_timer;
    int m_maxAreaNameLength;
    int m_currentDepth;
    
    static Profiler* m_instance;
};

#endif
