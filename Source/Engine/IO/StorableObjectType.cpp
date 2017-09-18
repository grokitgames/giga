
#include <giga-engine.h>

void StorableObjectType::AddField(std::string field, std::string friendly_name, bool editable, int type, std::string defaultValue) {
    StorableObjectField* nf = new StorableObjectField();
    nf->friendly_name = friendly_name;
    nf->name = field;
    nf->editable = editable;
    nf->type = type;
    nf->defaultValue = defaultValue;
    
    m_fields.push_back(nf);
}

void StorableObjectType::AddField(std::string field, int type, std::string defaultValue) {
    StorableObjectField* nf = new StorableObjectField();
    nf->friendly_name = "";
    nf->name = field;
    nf->editable = false;
    nf->type = type;
    nf->defaultValue = defaultValue;
    
    m_fields.push_back(nf);
}

void StorableObjectType::SetName(std::string name) {
    m_name = name;
}

void StorableObjectType::SetPrimaryKey(std::string name) {
    m_primaryKey = name;
}

std::vector<StorableObjectField*>& StorableObjectType::GetFields() {
    return(m_fields);
}

std::string StorableObjectType::GetFieldList() {
    std::string retval = "";
    if (m_fields.size() < 1) {
        return(retval);
    }
    
    int i = 0;
    /*retval = m_fields[0]->name;
    if (m_fields[0]->name.compare(m_primaryKey) == 0) {
        if (m_fields.size() > 1) {
            retval = m_fields[1]->name;
            i++;
        }
        else {
            retval = "";
        }
    }*/
    
    for (; i < m_fields.size(); i++) {
        if (m_fields[i]->name.compare(m_primaryKey) == 0) {
            continue;
        }
        
        retval += "," + m_fields[i]->name;
    }
    
    return(retval.substr(1));
}

int StorableObjectType::GetFieldCount() {
    int count = 0;
    for (int i = 0; i < m_fields.size(); i++) {
        if (m_fields[i]->name == m_primaryKey) {
            continue;
        }
        count++;
    }
    return(count);
}

StorableObject* StorableObjectType::CreateRecord() {
    return(m_createFunc());
}
