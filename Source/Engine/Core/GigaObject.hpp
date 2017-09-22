
#ifndef gigaobject_hpp
#define gigaobject_hpp

#define GIGA_CLASS_NAME(str) virtual std::string GetGigaName() { return str; } virtual std::string GetStorableTypeName() { return str; }

/**
 * Base object class for almost all objects in the engine
 */
class GIGA_API GigaObject {
public:
    GigaObject() = default;
    virtual ~GigaObject() = default;
    
    /**
     * Pure virtual function that specifies string version of class name
     * Use GIGA_CLASS_NAME("name") to set
     */
    virtual std::string GetGigaName() = 0;
    
    /**
     * Convert object to a string representation
     */
    virtual std::string ToString() { return "GigaObject []"; }
    
    /**
     * Lock/unlock access for a thread
     */
    virtual void LockMutex();
    virtual void UnlockMutex();
    
protected:
    // Locking for multi-threaded access
    std::mutex m_mutex;
};

#endif 
