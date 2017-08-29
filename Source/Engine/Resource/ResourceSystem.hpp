
#ifndef resourcesystem_hpp
#define resourcesystem_hpp

#define RESOURCE_DELETION_TIME      300 // 5 minutes

/**
 * Manage and find resources (typically anything on disk)
 */
class GIGA_API ResourceSystem : public System {
public:
    ResourceSystem() = default;
    ~ResourceSystem();
    
    GIGA_CLASS_NAME("ResourceSystem");
    
    // Internal function to call a C++ constructor
    typedef ResourceObject* (*ResourceObjectCreateFunc)();
    
    struct ResourceObjectType {
        std::string name;
        ResourceObjectCreateFunc createFunc;
    };

	/**
	 * Initialize
	 */
	void Initialize();
    
    /**
     * Update the resource system, discarding un-needed resources
     */
    void Update(double delta);
    
    /**
     * Add a new resource to the pool
     */
    void AddResource(ResourceObject* resource);
    
    /**
     * Remove a resource
     */
    void RemoveResource(ResourceObject* resource);
    
    /**
     * Load a resource
     */
    ResourceObject* LoadResource(std::string filename, std::string type);
    
    /**
     * Find a resource by filename
     */
    ResourceObject* Find(std::string filename);
    
    /**
     * Add a directory/path to search to find files
     */
    void AddSearchPath(std::string path);
    
    /**
     * Register a resource type to a class
     */
    template<class T>
    void RegisterResourceType(std::string className) {
        ResourceObjectType* type = new ResourceObjectType();
        type->name = className;
        type->createFunc = &CreateResourceObject<T>;
        m_resourceTypes.push_back(type);
    }
    
    /**
     * Scripting integration
     */
    static Variant* LoadResource(Variant* object, int argc, Variant** argv);
    
protected:
    /**
     * Iterate over search paths to find full path by filename
     */
    std::string FindResourcePath(std::string filename);
    
    // Create a new C++ object based on a registered script interface type
    template<typename T> static ResourceObject* CreateResourceObject() { return new T; }
    
protected:
    // A list of searchable paths
    std::vector<std::string> m_paths;
    
    // A list of registered resources
    ObjectPool<ResourceObject> m_resources;
    
    // A map of class names to create functions
    std::vector<ResourceObjectType*> m_resourceTypes;
};

#endif
