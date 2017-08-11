
#include <giga-engine.h>

SQLiteDataLoader::SQLiteDataLoader() {
    m_handle = 0;
}

SQLiteDataLoader::~SQLiteDataLoader() {
    if (m_handle) {
        sqlite3_close(m_handle);
    }
}

bool SQLiteDataLoader::Open(std::string filename) {
    int result = sqlite3_open(filename.c_str(), &m_handle);
    if (result != 0) {
        ErrorSystem::Process(new Error(ERROR_WARN, "Unable to ope SQLite file.", filename));
        sqlite3_close(m_handle);
        m_handle = 0;
        return(false);
    }
    
    return(true);
}

void SQLiteDataLoader::Close() {
    if (m_handle) {
        sqlite3_close(m_handle);
    }
}

int SQLiteDataLoader::InternalDataCallback(void* instance, int count, char** data, char** cols) {
    SQLiteDataLoader* obj = (SQLiteDataLoader*)instance;
    StorableObjectType* type = obj->m_tempType;
    
    unsigned int primaryKeyID = 0;
    StorableObject* record = type->CreateRecord();
    
    for (int i = 0; i < count; i++) {
        if (data[i] == 0) {
            continue;
        }
        
        record->UpdateStorableObjectFieldValue(cols[i], data[i]);
        
        if (strcmp(cols[i], type->GetPrimaryKey().c_str()) == 0) {
            primaryKeyID = atoi(data[i]);
        }
    }
    
    record->SetStorableObjectID(primaryKeyID);
    record->SetStorableObjectType(type);
    
    obj->m_records[type->GetName()].push_back(record);
    return(0);
}

std::vector<StorableObject*> SQLiteDataLoader::GetRecords(StorableObjectType* type, int sceneID) {
    UpdateTables();
    
    // Save the type
    m_tempType = type;
    
    // Get the name of the type we want to get
    std::string name = type->GetName();
    std::vector<StorableObject*> retval;
    
    // Erase any existing records of this type
    std::map< std::string, std::vector<StorableObject*> >::iterator i = m_records.find(name);
    if (i != m_records.end()) {
        i->second.clear();
    }
    
    // Get our scene ID
    char strSceneID[10];
    if (sceneID) {
        sprintf(strSceneID, "%d", sceneID);
    }
    
    // Construct our query
    std::string query = "SELECT ";
    std::vector<StorableObjectField*> fields = type->GetFields();
    query += type->GetPrimaryKey() + "," + type->GetFieldList() + " FROM " + name;
    
    if (sceneID) {
        query += " WHERE scene_id = ";
        query += strSceneID;
    }
    
    if (sqlite3_exec(m_handle, query.c_str(), InternalDataCallback, this, 0) != 0) {
        ErrorSystem::Process(new Error(ERROR_WARN, "Unable to get record list from SQLite.", (char*)sqlite3_errmsg(m_handle)));
        return(retval);
    }
    
    m_tempType = 0;
    return(m_records[name]);
}

void SQLiteDataLoader::UpdateTables() {
    // Create tables where they do not exist
    std::vector<StorableObjectType*>::iterator r = m_recordTypes.begin();
    for (; r != m_recordTypes.end(); r++) {
        // Get our primary key
        std::string primaryKey = (*r)->GetPrimaryKey();
        
        std::string query = "CREATE TABLE IF NOT EXISTS ";
        query += (*r)->GetName() + " (";
        
        query += primaryKey + " INTEGER,";
        
        std::vector<StorableObjectField*> fields = (*r)->GetFields();
        for (int j = 0; j < fields.size(); j++) {
            query += fields[j]->name + " TEXT";
            query += ",";
        }
        
        query += "PRIMARY KEY(" + primaryKey + " ASC))";
        
        if (sqlite3_exec(m_handle, query.c_str(), 0, 0, 0) != 0) {
            ErrorSystem::Process(new Error(ERROR_WARN, "Unable to create SQLite table.", (char*)sqlite3_errmsg(m_handle)));
        }
    }
    
    // Alter table as necessary where columns may not exist
    r = m_recordTypes.begin();
    for (; r != m_recordTypes.end(); r++) {
        std::string primaryKey = (*r)->GetPrimaryKey();
        
        // Get our column names
        std::string query = "PRAGMA table_info(" + (*r)->GetName() + ")";
        if (sqlite3_exec(m_handle, query.c_str(), 0, 0, 0) != 0) {
            ErrorSystem::Process(new Error(ERROR_WARN, "Unable to get SQLite schema.", (char*)sqlite3_errmsg(m_handle)));
            return;
        }
        
        // Put the column names into a vector
        sqlite3_stmt* stmt = 0;
        int result = 0;
        std::vector<std::string> existingCols;
        result = sqlite3_prepare(m_handle, query.c_str(), (int)query.length() + 1, &stmt, 0);
        while ((result = sqlite3_step(stmt))) {
            if (result != SQLITE_ROW) {
                break;
            }
            
            std::string colname = (const char*)sqlite3_column_text(stmt, 1);
            existingCols.push_back(colname);
        }
        
        std::vector<StorableObjectField*> fields = (*r)->GetFields();
        for (int j = 0; j < fields.size(); j++) {
            if (fields[j]->name == primaryKey) continue;
            
            std::vector<std::string>::iterator it = std::find(existingCols.begin(), existingCols.end(), fields[j]->name);
            if (it == existingCols.end()) {
                query = "ALTER TABLE " + (*r)->GetName() + " ADD COLUMN " + fields[j]->name;
                
                if (sqlite3_exec(m_handle, query.c_str(), 0, 0, 0) != 0) {
                    ErrorSystem::Process(new Error(ERROR_WARN, "Unable to alter SQLite table.", (char*)sqlite3_errmsg(m_handle)));
                    return;
                }
            }
        }
    }
}

void SQLiteDataLoader::SaveRecords() {
    UpdateTables();
    
    // Insert records
    std::map< std::string, std::vector<StorableObject*> >::iterator i = m_records.begin();
    for (; i != m_records.end(); i++) {
        for (int j = (int)i->second.size() - 1; j >= 0; j--) {
            // Get our record out
            StorableObject* record = i->second[j];
            std::string query = "";
            
            if (record->IsTransient()) {
                continue;
            }
            
            // Trigger an update of the record's values
            record->UpdateStorableObjectFieldValues();
            
            // Get record type
            StorableObjectType* type = record->GetStorableObjectType();
            
            // Get the primary key
            char primaryKeyID[10];
            sprintf(primaryKeyID, "%d", record->GetStorableObjectID());
            
            // Process deletion
            if (record->IsDeleted()) {
                // Construct query
                query = "DELETE FROM " + i->first + " WHERE " + type->GetPrimaryKey() + " = ";
                query += primaryKeyID;
                
                if (sqlite3_exec(m_handle, query.c_str(), 0, 0, 0) != 0) {
                    ErrorSystem::Process(new Error(ERROR_WARN, "Unable to delete object from SQLite.", (char*)sqlite3_errmsg(m_handle)));
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
            if (record->GetStorableObjectID() == 0) {
                query = "INSERT INTO " + i->first + " (";
                std::string values = ") VALUES (";
                for (std::vector<StorableObjectField*>::iterator i = fields.begin(); i != fields.end(); i++) {
                    std::string field = (*i)->name;
                    if (field.compare(primaryKey) != 0) {
                        query += field + ",";
                        values += "'" + record->GetStorableObjectFieldValue(field) + "',";
                    }
                }
                
                query = query.substr(0, query.length() - 1);
                values = values.substr(0, values.length() - 1);
                query += values + ")";
                
                if (sqlite3_exec(m_handle, query.c_str(), 0, 0, 0) != 0) {
                    ErrorSystem::Process(new Error(ERROR_WARN, "Unable to insert object from SQLite.", (char*)sqlite3_errmsg(m_handle)));
                    assert(false); // Probably need to break here so you don't keep repeating this
                }
                
                unsigned int newID = (unsigned int)sqlite3_last_insert_rowid(m_handle);
                if (newID == 0) {
                    ErrorSystem::Process(new Error(ERROR_WARN, "Unable to insert object from SQLite.", (char*)sqlite3_errmsg(m_handle)));
                    assert(false); // Probably need to break here so you don't keep repeating this
                }
                
                record->SetStorableObjectID(newID);
                continue;
            }
            
            // Update record
            query = "UPDATE " + i->first + " SET ";
            for (std::vector<StorableObjectField*>::iterator i = fields.begin(); i != fields.end(); i++) {
                std::string field = (*i)->name;
                if (field.compare(primaryKey) != 0) {
                    query += field + " = '" + record->GetStorableObjectFieldValue((*i)->name) + "', ";
                }
            }
            
            query = query.substr(0, query.length() - 2);
            query += " WHERE " + primaryKey + " = " + primaryKeyID;
            
            if (sqlite3_exec(m_handle, query.c_str(), 0, 0, 0) != 0) {
                ErrorSystem::Process(new Error(ERROR_WARN, "Unable to update object from SQLite.", (char*)sqlite3_errmsg(m_handle)));
                //assert(false); // Probably need to break here so you don't keep repeating this
            }
        }
    }
}

void SQLiteDataLoader::AddRecord(StorableObject* data) {
    StorableObjectType* type = data->GetStorableObjectType();
    std::string typeName = type->GetName();
    m_records[typeName].push_back(data);
}
