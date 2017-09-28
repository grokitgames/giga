
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
class GIGA_API Application : public ScriptableObject {
public:
    Application();
    ~Application();

	GIGA_CLASS_NAME("Application");
    
    struct RegisteredSystem {
        int tickRate;
        float accumulator;
        System* system;
    };
    
    /**
     * Initialize our application's default systems (must be called at the beginning of application)
     */
    void Initialize();

	/**
	 * Initialize per-thread specific resources
	 */
	void InitializeThread();
    
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
    T* CreateSystem(int tickRate = 0) {
        T* obj = new T();
        System* sys = dynamic_cast<System*>(obj);
        GIGA_ASSERT(sys != 0, "Class must be inherited from System type.");
        
        std::vector<RegisteredSystem*>::iterator i = m_systems.begin();
        for(; i != m_systems.end(); i++) {
            if((*i)->system->GetGigaName() == sys->GetGigaName()) {
                GIGA_ASSERT(false, "System type already registed.");
            }
        }
        
        RegisteredSystem* rs = new RegisteredSystem();
        rs->tickRate = tickRate;
        rs->system = sys;
        rs->accumulator = 0;
        m_systems.push_back(rs);
        
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
     * Find a specific subsystem by class type
     */
    template<class T>
    T* GetSystem() {
        std::vector<RegisteredSystem*>::iterator i = m_systems.begin();
        for (; i != m_systems.end(); i++) {
            T* object = dynamic_cast<T*>((*i)->system);
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

	/**
	* Scripting integration
	*/
	static Variant* Log(Variant* object, int argc, Variant** argv);
    
protected:
    // List of registered sub-systems
    std::vector<RegisteredSystem*> m_systems;
    
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
