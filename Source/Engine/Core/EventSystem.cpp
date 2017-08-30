
#include <giga-engine.h>

EventSystem::~EventSystem() {
    for(size_t i = 0; i < m_handlers.size(); i++) {
        delete m_handlers[i];
    }
    
    for(size_t i = 0; i < m_events.size(); i++) {
        delete m_events[i];
    }
}

void EventSystem::Process(Event* event) {
    // Find our event system
    EventSystem* eventSystem = GetSystem<EventSystem>();
    eventSystem->m_events.push_back(event);
}

void EventSystem::Update(float delta) {
    for(std::vector<Event*>::iterator i = m_events.begin(); i != m_events.end(); i++) {
        std::string type = (*i)->GetType();
        for(size_t j = 0; j < m_handlers.size(); j++) {
            if(m_handlers[j]->type == type || m_handlers[j]->type == "all") {
                if(m_handlers[j]->sender == 0 || m_handlers[j]->sender == (*i)->GetSender())
                    m_handlers[j]->func(*i);
            }
        }
    }
    
    m_events.clear();
}

void EventSystem::RegisterEventHandler(std::string type, EventHandlerFn func, GigaObject* sender) {
    // Make sure this combination doesn't already exist
    for(size_t j = 0; j < m_handlers.size(); j++) {
        if(m_handlers[j]->type == type && m_handlers[j]->func == func && m_handlers[j]->sender == sender)
            return;
    }
    
    // Create a new event handler
    EventHandler* handler = new EventHandler();
    handler->type = type;
    handler->sender = sender;
    handler->func = func;
    
    m_handlers.push_back(handler);
}

Variant* EventSystem::Process(Variant* object, int argc, Variant** argv) {
    GIGA_ASSERT(argc == 1, "Process function expects a single parameter.");
    GIGA_ASSERT(argv[0]->IsObject(), "Expecting a single parameter of type Event.");
    
    // Get global event system
    EventSystem* eventSystem = GetSystem<EventSystem>();
    
    // Get our event out
    Event* event = argv[0]->AsObject<Event>();

    // Dispatch
    eventSystem->Process(event);
    
    return(new Variant(0));
}

Variant* EventSystem::RegisterEventHandler(Variant* object, int argc, Variant** argv) {
    GIGA_ASSERT(argc == 2 || argc == 3, "RegisterEventHandler function expects 2 or 3 arguments.");
    GIGA_ASSERT(argv[0]->IsString(), "First parameter should be a string event type.");
    GIGA_ASSERT(argv[1]->IsString(), "Second parameter should be a function name.");
    
    // Get our sender object out if necessary
    GigaObject* sender = 0;
    if(argc > 2) {
        GIGA_ASSERT(argv[2]->IsObject(), "Third parameter should be a sender object.");
        sender = argv[2]->AsObject<GigaObject>();
    }
    
    // Pass to scripting system to handle
    ScriptingSystem* scriptingSystem = GetSystem<ScriptingSystem>();
    scriptingSystem->RegisterEventHandler(argv[0]->AsString(), argv[1]->AsString(), sender);
    
    return(new Variant(0));
}
