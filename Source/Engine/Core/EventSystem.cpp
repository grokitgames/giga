
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
	PROFILE_START_AREA("EventSystem Update");

	int events = m_events.size();
	for (int i = 0; i < events; i++) {
		std::string type = m_events[i]->GetType();
		for (size_t j = 0; j < m_handlers.size(); j++) {
			if (m_handlers[j]->type == type || m_handlers[j]->type == "all") {
				if (m_handlers[j]->entityID == 0 || m_handlers[j]->entityID == m_events[i]->GetEntityID())
					m_handlers[j]->func(m_events[i]);
			}
		}
	}
    
	m_events.erase(m_events.begin(), m_events.begin() + events);
	PROFILE_END_AREA("EventSystem Update");
}

void EventSystem::RegisterEventHandler(std::string type, EventHandlerFn func, int entityID) {
    // Make sure this combination doesn't already exist
    for(size_t j = 0; j < m_handlers.size(); j++) {
        if(m_handlers[j]->type == type && m_handlers[j]->func == func && m_handlers[j]->entityID == entityID)
            return;
    }
    
    // Create a new event handler
    EventHandler* handler = new EventHandler();
    handler->type = type;
    handler->entityID = entityID;
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

Variant* EventSystem::RegisterEventHandler(ScriptComponent* component, Variant* object, int argc, Variant** argv) {
    GIGA_ASSERT(argc == 2 || argc == 3, "RegisterEventHandler function expects 2 or 3 arguments.");
    GIGA_ASSERT(argv[0]->IsString(), "First parameter should be a string event type.");
    GIGA_ASSERT(argv[1]->IsString(), "Second parameter should be a function name.");
    
    // Get our sender object out if necessary
    int entityID = 0;
    if(argc > 2) {
        GIGA_ASSERT(argv[2]->IsInt(), "Third parameter should be an entity ID.");
        entityID = argv[2]->AsInt();
    }
    
    // Pass to scripting system to handle
    ScriptingSystem* scriptingSystem = GetSystem<ScriptingSystem>();
    scriptingSystem->RegisterEventHandler(component, argv[0]->AsString(), argv[1]->AsString(), entityID);
    
    return(new Variant(0));
}
