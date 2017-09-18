
#ifndef dataloader_hpp
#define dataloader_hpp

/**
 * A virtual base class for other loaders and savers
 */
class GIGA_API DataLoader : public ScriptableObject {
public:
    DataLoader() { }
    ~DataLoader() { }
    
    /**
     * Return a list of stored records
     */
	virtual std::vector<StorableObject*> GetRecords(std::string type , int sceneID);
    
    /**
     * Save (includes updates, additions and deletions) records
     */
    virtual void SaveRecord(StorableObject* record) { }
    
    /**
     * Add a new record
     */
    virtual void AddRecord(StorableObject* data) { }
    
    /**
     * Create or alter any necessary tables for registered types
     */
    virtual void UpdateTables() { }
    
    /**
     * Register record type
     */
    template<typename T> static void RegisterRecordType(StorableObjectType* type) {
        type->m_createFunc = &CreateRecord<T>;
        m_recordTypes.push_back(type);
    }
    
    static StorableObjectType* GetRecordType(std::string type);

	static std::vector<StorableObjectType*>& GetRecordTypes() { return m_recordTypes; }
    
protected:
    // Our internal pointer to records
    std::map< std::string, std::vector<StorableObject*> > m_records;
    
    // Our registered record types
    static std::vector<StorableObjectType*> m_recordTypes;
    
    template<typename T> static StorableObject* CreateRecord() { return new T; }
};

#endif
