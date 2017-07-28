
#include <giga-engine.h>

Event::~Event() {
    delete m_data;
}

Variant* Event::GetType(Variant* object) {
    // Cast to event object
    Event* event = object->AsObject<Event>();
    
    return(new Variant(event->GetType()));
}

Variant* Event::GetData(Variant* object) {
    // Cast to event object
    Event* event = object->AsObject<Event>();
    
    return(new Variant(event->GetData()));
}

Variant* Event::GetSender(Variant* object) {
    // Cast to event object
    Event* event = object->AsObject<Event>();
    
    return(new Variant(event->GetSender()));
}
