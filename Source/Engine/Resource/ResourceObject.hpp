
#ifndef resourceobject_hpp
#define resourceobject_hpp

/**
 * An object which contains a reference to a Resource object
 */
class GIGA_API ResourceObject : public ScriptableObject {
public:
    ResourceObject();
    virtual ~ResourceObject();
    
    GIGA_CLASS_NAME("ResourceObject");
    
    /**
     * Get our binary file data from our resource object
     */
    unsigned char* GetData();
    
    /**
     * Get resource object
     */
    Resource* GetResource() { return m_resource; }
    
    /**
     * Post processing of the file data
     */
    virtual void ProcessData() { }
    
    // Allow the resource system to set the internal resource
    friend class ResourceSystem;
    
protected:
    // The resource this object is using
    Resource* m_resource;
};

#endif
