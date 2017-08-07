
#ifndef storableobject_hpp
#define storableobject_hpp

class GIGA_API StorableObject {
public:
    StorableObject();
    virtual ~StorableObject() = default;
    
    /**
     * Update the data field mappings to bind to member variables
     */
    virtual void UpdateObjectDataMappings() { }
    
    /**
     * Serialize the object's field mappings into a binary format
     */
    virtual unsigned char* Serialize(int& size);
    
    /**
     * Deserialize the object's field mappings from a binary format
     */
    virtual void Deserialize(unsigned char* data, int size);
    
    /**
     * Get serialized data size
     */
    virtual int GetSerializedSize();
    
    /**
     * Update the data fields (based on member fields)
     */
    void UpdateStorableObjectFieldValues();
    
    /**
     * Get/set stored record type
     */
    StorableObjectType* GetStorableObjectType() { return m_storableObjectType; }
    void SetStorableObjectType(StorableObjectType* type) { m_storableObjectType = type; }
    
    /**
     * Get/set field values
     */
    void UpdateStorableObjectFieldValue(std::string field, std::string value);
    std::string GetStorableObjectFieldValue(std::string field);
    
    /**
     * Set a record as transient (non-savable)
     */
    void SetObjectAsTransient(bool transient) { m_transient = transient; }
    bool IsTransient() { return m_transient; }
    
    /**
     * Get/set the record ID
     */
    unsigned int GetStorableObjectID() { return m_storableObjectID; }
    void SetStorableObjectID(unsigned int id) { m_storableObjectID = id; }
    
    /**
     * Mark the record as deleted (for the next sync)
     */
    void DeleteRecord(bool deleted) { m_recordDeleted = deleted; }
    bool IsDeleted() { return m_recordDeleted; }
    
protected:
    /**
     * Set mappings
     */
    void SetStorableObjectFieldMapping(std::string field, std::string* mapping);
    void SetStorableObjectFieldMapping(std::string field, int* mapping);
    void SetStorableObjectFieldMapping(std::string field, float* mapping);
    void SetStorableObjectFieldMapping(std::string field, vector2* mapping);
    void SetStorableObjectFieldMapping(std::string field, vector3* mapping);
    void SetStorableObjectFieldMapping(std::string field, quaternion* mapping);
    
private:
    // The interface object for our type
    StorableObjectType* m_storableObjectType;
    
    // Stored record ID (primary key value)
    uint32_t m_storableObjectID;
    
    // Whether this record is transient or not (non-savable)
    bool m_transient;
    
    // Our stored field values
    std::map<std::string, std::string> m_storableRecordValues;
    
    // A list of mappings to data fields (based on type)
    std::map<std::string, std::string*> m_storableObjectStringFields;
    std::map<std::string, int*> m_storableObjectIntFields;
    std::map<std::string, float*> m_storableObjectFloatFields;
    std::map<std::string, vector2*> m_storableObjectVector2Fields;
    std::map<std::string, vector3*> m_storableObjectVector3Fields;
    std::map<std::string, quaternion*> m_storableObjectQuaternionFields;
    
    // Mark the record as deleted
    bool m_recordDeleted;
};

#endif
