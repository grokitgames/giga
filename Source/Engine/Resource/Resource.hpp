
#ifndef resource_hpp
#define resource_hpp

/**
 * Resource base class
 */
class GIGA_API Resource : public GigaObject {
public:
    Resource();
    ~Resource();
    
    GIGA_CLASS_NAME("Resource");
    
    /**
     * Get filename/full path
     */
    std::string GetFilename() { return filename; }
    std::string GetFullPath() { return m_fullpath; }
    std::string GetExtension() { return m_extension; }
    
    /**
     * Set full path (parse filename)
     */
    void SetFilename(std::string filename);
    
    /**
     * Load data
     */
    void Load();
    
    /**
     * Unload data
     */
    void Unload();
    
    /**
     * Get the data out (sets last access time)
     */
    unsigned char* GetData();
    
    /**
     * Other getter functions
     */
    time_t GetLastAccess() { return m_lastAccessTime; }
    int GetFileSize() { return m_filesize; }
    bool IsLoaded() { return m_loaded; }
    
    /**
     * Get resource as a string (filename)
     */
    std::string ToString() { return filename; }

	/**
	 * Get/set type
	 */
	void SetType(std::string type) { m_type = type; }
	std::string GetType() { return m_type; }
    
public:
    // Filename
    std::string filename;
    
protected:
    // Full path + filename
    std::string m_fullpath;
    
    // File extension
    std::string m_extension;
    
    // Binary file data
    unsigned char* m_data;
    
    // File size
    int m_filesize;
    
    // Whether this resource is loaded or not
    bool m_loaded;
    
    // Last access time
    time_t m_lastAccessTime;

	// Type
	std::string m_type;
};

#endif
