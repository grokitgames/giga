
#ifndef errorsystem_hpp
#define errorsystem_hpp

/**
 * Error handling function
 */
class Error;
typedef void(*ErrorHandlerFn)(Error* error);

/**
 * Error handling system
 */
class GIGA_API ErrorSystem : public System {
public:
    ErrorSystem() = default;
    ~ErrorSystem();
    
    GIGA_CLASS_NAME("ErrorSystem");
  
    /**
     * Static error handling function (eg. ErrorSystem::Process(...))
     */
    static void Process(Error* error);
    
    /**
     * Register a new error handling function
     */
    void RegisterErrorHandler(int type, ErrorHandlerFn func);
    
private:
    /**
     * Error handler
     */
    struct ErrorHandler {
        int type;
        ErrorHandlerFn func;
    };
    
private:
    // List of error handlers
    std::vector<ErrorHandler*> m_handlers;
};

#endif
