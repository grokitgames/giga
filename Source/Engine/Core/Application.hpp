
#ifndef application_hpp
#define application_hpp

#ifdef WIN32
#define GIGA_APPLICATION(func) int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) { func(); }
#else
#define GIGA_APPLICATION(func) int main(int argc, const char * argv[]) { func(); }
#endif

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
    void Initialize();
    
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
    template<class T>
    T* CreateSystem() {
        T* obj = new T();
        System* sys = dynamic_cast<System*>(obj);
        GIGA_ASSERT(sys != 0, "Class must be inherited from System type.");
        
        for(size_t i = 0; i < m_systems.size(); i++) {
            if(m_systems[i]->GetGigaName() == sys->GetGigaName()) {
                GIGA_ASSERT(false, "System type already registed.");
            }
        }
        
        sys->Initialize();
        m_systems.push_back(sys);
        return(obj);
    }
    
    /**
     * Register a new data loader
     */
    template<class T>
    T* CreateDataLoader() {
        T* obj = new T();
        DataLoader* loader = dynamic_cast<DataLoader*>(obj);
        GIGA_ASSERT(loader != 0, "Class must be inherited from DataLoader type.");
        
        for(size_t i = 0; i < m_loaders.size(); i++) {
            if(m_loaders[i]->GetGigaName() == loader->GetGigaName()) {
                GIGA_ASSERT(false, "System type already registed.");
            }
        }
        
        m_loaders.push_back(loader);
        return(obj);
    }
    
    /**
     * Update registered sub-systems
     */
    void Update(float delta);
    
    /**
     * Get sub-systems
     */
    std::vector<System*>& GetSystems() { return m_systems; }
    
    /**
     * Find a specific subsystem by class type
     */
    template<class T>
    T* GetSystem() {
        for (size_t i = 0; i < m_systems.size(); i++) {
            T* object = dynamic_cast<T*>(m_systems[i]);
            if (object) {
                return(object);
            }
        }
        
        return(0);
    }
    
    /**
     * Find a specific data loader by class type
     */
    template<class T>
    T* GetDataLoader() {
        for (size_t i = 0; i < m_loaders.size(); i++) {
            T* object = dynamic_cast<T*>(m_loaders[i]);
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
    
    // List of registered data loaders
    std::vector<DataLoader*> m_loaders;
    
    // Singleton
    static Application* m_instance;
    
    // Main application window
    Window* m_window;
    
    // Debug/output log
    File* m_outputLog;
    
    // Minimum logging level
    int m_loggingLevel;
};

// Short-hand class to find sub-systems in the Application class
template<class T> T* GetSystem() {
    Application* application = Application::GetInstance();
    return(application->GetSystem<T>());
}

// Short-hand class to find data loaders in the Application class
template<class T> T* GetDataLoader() {
    Application* application = Application::GetInstance();
    return(application->GetDataLoader<T>());
}

#endif
