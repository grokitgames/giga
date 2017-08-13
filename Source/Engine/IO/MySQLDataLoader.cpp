
#include <giga-engine.h>

bool MySQLDataLoader::Connect(std::string url, std::string username, std::string password, std::string db) {
    m_connection = mysql_init(NULL);
    
    m_url = url;
    m_username = username;
    m_password = password;
    m_db = db;
    
    if(mysql_real_connect(m_connection, url.c_str(), username.c_str(), password.c_str(), db.c_str(), 0, NULL, 0) == NULL) {
        ErrorSystem::Process(new Error(ERROR_WARN, "Unable to connect to MySQL", (char*)mysql_error(m_connection)));
        mysql_close(m_connection);
        return(false);
    }
    
    return(true);
}

void MySQLDataLoader::UpdateTables() {
    // Create tables where they do not exist
    std::vector<StorableObjectType*>::iterator r = m_recordTypes.begin();
    for(; r != m_recordTypes.end(); r++) {
        // Get our primary key
        std::string primaryKey = (*r)->GetPrimaryKey();
        
        std::string query = "CREATE TABLE IF NOT EXISTS ";
        query += (*r)->GetName() + " (";
        
        query += primaryKey + " INTEGER,";
        
        std::vector<StorableObjectField*> fields = (*r)->GetFields();
        for(int j = 0; j < fields.size(); j++) {
            query += fields[j]->name + " TEXT";
            query += ",";
        }
        
        query += "PRIMARY KEY(" + primaryKey + " ASC))";
        
        if(mysql_query(m_connection, query.c_str())) {
            ErrorSystem::Process(new Error(ERROR_WARN, "Unable to create MySQL table.", (char*)mysql_error(m_connection)));
            assert(false);
        }
    }
    
    // Alter table as necessary where columns may not exist
    r = m_recordTypes.begin();
    for(; r != m_recordTypes.end(); r++) {
        std::string primaryKey = (*r)->GetPrimaryKey();
        
        // Get our column names
        std::string query = "SHOW COLUMNS FROM " + (*r)->GetName();
        if(mysql_query(m_connection, query.c_str())) {
            ErrorSystem::Process(new Error(ERROR_WARN, "Unable to get MySQL schema.", (char*)mysql_error(m_connection)));
            return;
        }
        
        MYSQL_RES *result = mysql_store_result(m_connection);
        if (result == NULL) {
            ErrorSystem::Process(new Error(ERROR_WARN, "Unable to get record list from MySQL result.", (char*)mysql_error(m_connection)));
            assert(false);
        }
        
        MYSQL_ROW row;
        std::vector<std::string> existingCols;
        while((row = mysql_fetch_row(result))) {
            std::string colname = row[0];
            existingCols.push_back(colname);
        }
        
        std::vector<StorableObjectField*> fields = (*r)->GetFields();
        for(int j = 0; j < fields.size(); j++) {
            if(fields[j]->name == primaryKey) continue;
            
            std::vector<std::string>::iterator it = std::find(existingCols.begin(), existingCols.end(), fields[j]->name);
            if(it == existingCols.end()) {
                query = "ALTER TABLE " + (*r)->GetName() + " ADD COLUMN " + fields[j]->name;
                
                if(mysql_query(m_connection, query.c_str())) {
                    ErrorSystem::Process(new Error(ERROR_WARN, "Unable to alter MySQL table.",     (char*)mysql_error(m_connection)));
                    return;
                }
            }
        }
    }
}

std::vector<StorableObject*> MySQLDataLoader::GetRecords(StorableObjectType* type, int sceneID) {
    UpdateTables();
    
    // Get the name of the type we want to get
    std::string name = type->GetName();
    std::vector<StorableObject*> retval;
    
    // Erase any existing records of this type
    std::map< std::string, std::vector<StorableObject*> >::iterator i = m_records.find(name);
    if(i != m_records.end()) {
        i->second.clear();
    }
    
    // Get our scene ID
    char strSceneID[10];
    if(sceneID) {
        sprintf(strSceneID, "%d", sceneID);
    }
    
    // Construct our query
    std::string query = "SELECT ";
    std::vector<StorableObjectField*> fields = type->GetFields();
    query += type->GetPrimaryKey() + "," + type->GetFieldList() + " FROM " + name;
    
    if(sceneID) {
        query += " WHERE scene_id = ";
        query += strSceneID;
    }
    
    if (mysql_query(m_connection, query.c_str())) {
        ErrorSystem::Process(new Error(ERROR_WARN, "Unable to get record list from MySQL.", (char*)mysql_error(m_connection)));
        return(retval);
    }
    
    MYSQL_RES *result = mysql_store_result(m_connection);
    if (result == NULL) {
        ErrorSystem::Process(new Error(ERROR_WARN, "Unable to get record list from MySQL result.", (char*)mysql_error(m_connection)));
        return(retval);
    }
    
    int colCount = type->GetFieldCount();
    
    MYSQL_ROW row;
    while((row = mysql_fetch_row(result))) {
        unsigned int primaryKeyID = 0;
        StorableObject* record = type->CreateRecord();
        record->InitializeStorableObject(type->GetName());
        std::map<std::string, std::string> data;
        
        for(int i = 0; i < colCount; i++) {
            data[fields[i]->name] = row[i];
            record->UpdateStorableObjectFieldValue(fields[i]->name, row[i]);
            
            if(fields[i]->name.compare(type->GetPrimaryKey().c_str()) == 0) {
                primaryKeyID = atoi(row[i]);
            }
        }
        
        record->SetStorableObjectID(primaryKeyID);
        record->SetStorableObjectType(type);
        
        m_records[name].push_back(record);
    }
    
    mysql_free_result(result);
    return(m_records[name]);
}

void MySQLDataLoader::SaveRecords() {
    UpdateTables();
    
    std::map< std::string, std::vector<StorableObject*> >::iterator i = m_records.begin();
    for(; i != m_records.end(); i++) {
        for(int j = (int)i->second.size(); j >= 0; j--) {
            // Get our record out
            StorableObject* record = i->second[j];
            std::string query = "";
            
            // Trigger an update of the record's values
            record->UpdateStorableObjectFieldValues();
            
            // Get record type
            StorableObjectType* type = record->GetStorableObjectType();
            
            // Get the primary key
            char primaryKeyID[10];
            sprintf(primaryKeyID, "%d", record->GetStorableObjectID());
            
            // Process deletion
            if(record->IsDeleted()) {
                // Construct query
                query = "DELETE FROM " + i->first + " WHERE " + type->GetPrimaryKey() + " = ";
                query += primaryKeyID;
                
                if (mysql_query(m_connection, query.c_str())) {
                    ErrorSystem::Process(new Error(ERROR_WARN, "Unable to delete object from MySQL.", (char*)mysql_error(m_connection)));
                    assert(false); // Probably need to break here so you don't keep repeating this
                }
                
                // Remove from our records as well
                i->second.erase(std::remove(i->second.begin(), i->second.end(), record), i->second.end());
                
                continue;
            }
            
            // Get fields to update
            std::vector<StorableObjectField*> fields = type->GetFields();
            std::vector<std::string> pairs;
            std::string primaryKey = type->GetPrimaryKey();
            
            // Process new records
            if(record->GetStorableObjectID() == 0) {
                query = "INSERT INTO " + i->first + " (";
                std::string values = ") VALUES (";
                for(std::vector<StorableObjectField*>::iterator i = fields.begin(); i != fields.end(); i++) {
                    std::string field = (*i)->name;
                    if(field.compare(primaryKey) != 0) {
                        query += field + ",";
                        values += "'" + record->GetStorableObjectFieldValue((*i)->name) + "',";
                    }
                }
                
                query = query.substr(0, query.length() - 1);
                values = values.substr(0, values.length() - 1);
                query += values + ")";
                
                if (mysql_query(m_connection, query.c_str())) {
                    ErrorSystem::Process(new Error(ERROR_WARN, "Unable to update object from MySQL.", (char*)mysql_error(m_connection)));
                    assert(false); // Probably need to break here so you don't keep repeating this
                }
                
                unsigned int newID = (unsigned int)mysql_insert_id(m_connection);
                if(newID == 0) {
                    ErrorSystem::Process(new Error(ERROR_WARN, "Unable to insert object from MySQL.", (char*)mysql_error(m_connection)));
                    assert(false); // Probably need to break here so you don't keep repeating this
                }
                
                record->SetStorableObjectID(newID);
                continue;
            }
            
            // Update record
            query = "UPDATE " + i->first + " SET ";
            for(std::vector<StorableObjectField*>::iterator i = fields.begin(); i != fields.end(); i++) {
                std::string field = (*i)->name;
                if(field.compare(primaryKey) != 0) {
                    query += field + " = '" + record->GetStorableObjectFieldValue((*i)->name) + "', ";
                }
            }
            
            query = query.substr(0, query.length() - 2);
            query += " WHERE " + primaryKey + " = " + primaryKeyID;
            
            if (mysql_query(m_connection, query.c_str())) {
                ErrorSystem::Process(new Error(ERROR_WARN, "Unable to update object from MySQL.", (char*)mysql_error(m_connection)));
                assert(false); // Probably need to break here so you don't keep repeating this
            }
        }
    }
}

void MySQLDataLoader::AddRecord(StorableObject* data) {
    StorableObjectType* type = data->GetStorableObjectType();
    std::string typeName = type->GetName();
    m_records[typeName].push_back(data);
}
