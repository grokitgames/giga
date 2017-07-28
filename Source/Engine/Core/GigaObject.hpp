
#ifndef gigaobject_hpp
#define gigaobject_hpp

#define GIGA_CLASS_NAME(str) virtual std::string GetClassName() { return str; }

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
    virtual std::string GetClassName() = 0;
    
    /**
     * Convert object to a string representation
     */
    virtual std::string ToString() { return "GigaObject []"; }
};

#endif 
