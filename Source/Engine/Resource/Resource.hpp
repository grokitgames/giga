
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
    std::string GetFilename() { return m_filename; }
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
    std::string GetString();
    
    /**
     * Other getter functions
     */
    time_t GetLastAccess() { return m_lastAccessTime; }
    int GetFileSize() { return m_filesize; }
    bool IsLoaded() { return m_loaded; }
    
    /**
     * Get resource as a string (filename)
     */
    std::string ToString() { return m_filename; }
    
protected:
    // Filename
    std::string m_filename;
    
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
};

#endif
