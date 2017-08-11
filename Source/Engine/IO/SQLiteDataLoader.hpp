
#ifndef sqlitedataloader_hpp
#define sqlitedataloader_hpp

class GIGA_API SQLiteDataLoader : public DataLoader {
public:
    SQLiteDataLoader();
    ~SQLiteDataLoader();
    
    GIGA_CLASS_NAME("SQLiteDataLoader");
    
    // Open a SQLite file
    bool Open(std::string filename);
    
    // Close file
    void Close();
    
    /**
     * Return a list of stored records
     */
    std::vector<StorableObject*> GetRecords(StorableObjectType* type, int sceneID);
    
    /**
     * Save (includes updates, additions and deletions) records
     */
    void SaveRecords();
    
    /**
     * Add a new record
     */
    void AddRecord(StorableObject* data);
    
    /**
     * Create or alter any necessary tables for registered types
     */
    void UpdateTables();
    
    /**
     * Data callback
     */
    static int InternalDataCallback(void* instance, int count, char** data, char** cols);
    
protected:
    std::string m_filename;
    sqlite3* m_handle;
    
    // The type of record we are currently loading
    StorableObjectType* m_tempType;
};

#endif
