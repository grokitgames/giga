
#include <giga-engine.h>

Event::~Event() {
    delete m_data;
}

Variant* Event::GetType(std::string var, Variant* object) {
    // Cast to event object
    Event* event = object->AsObject<Event>();
    
    return(new Variant(event->GetType()));
}

Variant* Event::GetData(std::string var, Variant* object) {
    // Cast to event object
    Event* event = object->AsObject<Event>();
    
    return(new Variant(event->GetData()));
}

Variant* Event::GetEntityID(std::string var, Variant* object) {
    // Cast to event object
    Event* event = object->AsObject<Event>();
    return(new Variant(event->GetEntityID()));
}
