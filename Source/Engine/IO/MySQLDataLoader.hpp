
#ifndef mysqldataloader_hpp
#define mysqldataloader_hpp

class MySQLDataLoader : public DataLoader {
public:
    MySQLDataLoader() = default;
    ~MySQLDataLoader() = default;
    
    GIGA_CLASS_NAME("MySQLDataLoader");
    
    // Connect to a MySQL server
    bool Connect(std::string url, std::string username, std::string password, std::string db);
    
    /**
     * Return a list of stored records
     */
    std::vector<StorableObject*> GetRecords(std::string typeName, int sceneID);
    
    /**
     * Save (includes updates, additions and deletions) records
     */
    void SaveRecord(StorableObject* record);
    
    /**
     * Add a new record
     */
    void AddRecord(StorableObject* data);
    
    /**
     * Create or alter any necessary tables for registered types
     */
    void UpdateTables();
    
protected:
    std::string m_url, m_username, m_password, m_db;
    MYSQL *m_connection;
};

#endif
