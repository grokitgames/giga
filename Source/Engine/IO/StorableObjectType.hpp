
#ifndef storableobjecttype_hpp
#define storableobjecttype_hpp

class StorableObject;
typedef StorableObject* (*StorableObjectCreateFunc)(void);

class GIGA_API StorableObjectType {
public:
    StorableObjectType() = default;
    ~StorableObjectType() = default;
    
    /**
     * Add a field (with friendly name)
     */
    void AddField(std::string field, std::string friendly_name, bool editable, int type, std::string defaultValue = "");
    
    /**
     * Add a field (without friendly name)
     */
    void AddField(std::string field, int type, std::string defaultValue = "");
    
    /**
     * Return a concatenated list of fields names
     */
    std::string GetFieldList();
    
    /**
     * Return field names in a vector
     */
    std::vector<StorableObjectField*>& GetFields();
    int GetFieldCount();
    
    /**
     * Set the name of this object type (table name in SQL)
     */
    std::string GetName() { return m_name; }
    void SetName(std::string name);
    
    /**
     * Set the primary key/record identifier
     */
    void SetPrimaryKey(std::string name);
    std::string& GetPrimaryKey() { return m_primaryKey; }
    
    /**
     * Create a new record of a type
     */
    StorableObject* CreateRecord();
    
    friend class DataLoader;
    
protected:
    // A list of fields applicable to this type
    std::vector<StorableObjectField*> m_fields;
    
    // The name of this type
    std::string m_name;
    
    // The friendly name of this type
    std::string m_friendlyName;
    
    // The primary key (record ID of this type)
    std::string m_primaryKey;
    
    // Constructor for this object type
    StorableObjectCreateFunc m_createFunc;
};

#endif 
