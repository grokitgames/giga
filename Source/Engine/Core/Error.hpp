
#ifndef error_hpp
#define error_hpp

/**
 * Error levels
 */
enum GIGA_API {
    MSGTYPE_NONE = 0,
    MSGTYPE_DEBUG,
    MSGTYPE_INFO,
    MSGTYPE_WARN,
    MSGTYPE_FATAL
};

enum GIGA_API {
    ERROR_NONE = 0,
    ERROR_DEBUG,
    ERROR_INFO,
    ERROR_WARN,
    ERROR_FATAL
};

/**
 * Generic error handling class - passed to error system for handling
 */
class GIGA_API Error : public GigaObject {
public:
    Error(int type, std::string message) :
        m_type(type),
        m_message(message) { }
    
    Error(int type, std::string message, std::string details) :
        m_type(type),
        m_message(message),
        m_details(details) { }
    
    ~Error() = default;
    
    GIGA_CLASS_NAME("Error");
    
    /**
     * Getter functions
     */
    int GetType() { return m_type; }
    std::string GetMsg() { return m_message; }
    std::string GetDetails() { return m_details; }
    
protected:
    // Error level/type
    int m_type;
    
    // Error string
    std::string m_message;
    
    // Details (like a filename)
    std::string m_details;
};

#endif
