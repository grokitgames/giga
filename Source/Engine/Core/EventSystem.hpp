
#ifndef eventsystem_hpp
#define eventsystem_hpp

/**
 * Generic event handling function
 */
typedef void(*EventHandlerFn)(Event* event);

/**
 * Event handling system
 */
class GIGA_API EventSystem : public System {
public:
    EventSystem() = default;
    ~EventSystem();
    
    GIGA_CLASS_NAME("EventSystem");
    
    /**
     * Process an event (eg. EventSystem::Process(...))
     */
    static void Process(Event* event);
    
    /**
     * Update function - must be called to actually process events
     */
    void Update(float delta);
    
    /**
     * Register a general (global) event handler
     */
    void RegisterEventHandler(std::string type, EventHandlerFn func, GigaObject* sender = 0);
    
    /**
     * Scripting integration
     */
    static Variant* Process(Variant* object, int argc, Variant** argv);
    
    /**
     * Register a scripted event handler
     */
    static Variant* RegisterEventHandler(Variant* object, int argc, Variant** argv);
    
private:
    // The storage of an event handler
    struct EventHandler {
        std::string type;
        GigaObject* sender;
        EventHandlerFn func;
    };
    
private:
    // List of registered event handlers
    std::vector<EventHandler*> m_handlers;
    
    // Queue of events to be processed next update
    std::vector<Event*> m_events;
};

#endif 
