
#ifndef application_hpp
#define application_hpp

/**
 * Application class that serves as a service locator
 */
class GIGA_API Application {
public:
    Application();
    ~Application();
    
    /**
     * Initialize our application's default systems (must be called at the beginning of application)
     */
    void Initialize(std::string name = "");
    
    /**
     * Get/set window if we created one in our application
     */
    Window* GetPrimaryWindow() { return m_window; }
    void SetPrimaryWindow(Window* window) { m_window = window; }

    /**
     * Get singleton instance
     */
    static Application* GetInstance();
    
    /**
     * Register a new sub-system
     */
    void RegisterSystem(System* system);
    
    /**
     * Get sub-systems
     */
    std::vector<System*>& GetSystems() { return m_systems; }
    
    /**
     * Find a specific substance by class type
     */
    template<class T>
    T* GetSystem() {
        for (int i = 0; i < m_systems.size(); i++) {
            T* object = dynamic_cast<T*>(m_systems[i]);
            if (object) {
                return(object);
            }
        }
        
        return(0);
    }
    
    /**
     * Shutdown (calls Shutdown on all registered systems)
     */
    void Shutdown();
    
    /**
     * Log something to output file
     */
    static void Log(int type, std::string message, std::string details = "");
    
    /**
     * Default logging error hander
     */
    static void LogError(Error* error);
    
    /**
     * Set minimum debug level (should probably be ERROR_WARN for maybe ERROR_INFO for production)
     */
    void SetLoggingLevel(int level) { m_loggingLevel = level; }
    
protected:
    // List of registered sub-systems
    std::vector<System*> m_systems;
    
    // Singleton
    static Application* m_instance;
    
    // Main application window
    Window* m_window;
    
    // Debug/output log
    File* m_outputLog;
    
    // Application name
    std::string m_name;
    
    // Minimum logging level
    int m_loggingLevel;
};

// Short-hand class to find sub-systems in the Application class
template<class T> T* GetSystem() {
    Application* application = Application::GetInstance();
    return(application->GetSystem<T>());
}

#endif
