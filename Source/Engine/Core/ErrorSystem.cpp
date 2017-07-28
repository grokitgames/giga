
#include <giga-engine.h>

ErrorSystem::~ErrorSystem() {
    for(size_t i = 0; i < m_handlers.size(); i++) {
        delete m_handlers[i];
    }
}

void ErrorSystem::Process(Error* error) {
    // Get our error system
    ErrorSystem* errorSystem = GetSystem<ErrorSystem>();
    
    int type = error->GetType();
    for(size_t i = 0; i < errorSystem->m_handlers.size(); i++) {
        if(errorSystem->m_handlers[i]->type == type) {
            errorSystem->m_handlers[i]->func(error);
        }
    }
}

void ErrorSystem::RegisterErrorHandler(int type, ErrorHandlerFn func) {
    // Create a new error handler to add to the list
    ErrorHandler* handler = new ErrorHandler();
    handler->type = type;
    handler->func = func;
    
    // Add to list
    m_handlers.push_back(handler);
}
