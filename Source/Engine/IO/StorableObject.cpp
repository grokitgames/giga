
#include <giga-engine.h>

StorableObject::StorableObject() {
    m_transient = false;
	m_storableObjectType = 0;
}

void StorableObject::SetStorableObjectFieldMapping(std::string field, std::string* mapping) {
    m_storableObjectStringFields[field] = mapping;
}

void StorableObject::SetStorableObjectFieldMapping(std::string field, ResourceObject* mapping) {
	m_storableObjectResourceFields[field] = mapping;
}

void StorableObject::SetStorableObjectFieldMapping(std::string field, int* mapping) {
    m_storableObjectIntFields[field] = mapping;
}

void StorableObject::SetStorableObjectFieldMapping(std::string field, float* mapping) {
    m_storableObjectFloatFields[field] = mapping;
}

void StorableObject::SetStorableObjectFieldMapping(std::string field, vector2* mapping) {
    m_storableObjectVector2Fields[field] = mapping;
}

void StorableObject::SetStorableObjectFieldMapping(std::string field, vector3* mapping) {
	std::map<std::string, vector3*>::iterator i = m_storableObjectVector3Fields.find(field);
	GIGA_ASSERT(i == m_storableObjectVector3Fields.end(), "Field mapping already exists.");
    m_storableObjectVector3Fields[field] = mapping;
}

void StorableObject::SetStorableObjectFieldMapping(std::string field, quaternion* mapping) {
    m_storableObjectQuaternionFields[field] = mapping;
}

void StorableObject::UpdateStorableObjectFieldValues() {
    for (std::map<std::string, std::string*>::iterator i = m_storableObjectStringFields.begin(); i != m_storableObjectStringFields.end(); i++) {
        m_storableRecordValues[i->first] = *(i->second);
    }
    
    for (std::map<std::string, int*>::iterator i = m_storableObjectIntFields.begin(); i != m_storableObjectIntFields.end(); i++) {
        char value[50];
        sprintf(value, "%d", *(i->second));
        m_storableRecordValues[i->first] = value;
    }
    
    for (std::map<std::string, float*>::iterator i = m_storableObjectFloatFields.begin(); i != m_storableObjectFloatFields.end(); i++) {
        char value[50];
        sprintf(value, "%.2f", *(i->second));
        m_storableRecordValues[i->first] = value;
    }
    
    for (std::map<std::string, vector2*>::iterator i = m_storableObjectVector2Fields.begin(); i != m_storableObjectVector2Fields.end(); i++) {
        char value[50];
        vector2 vec = *(i->second);
        sprintf(value, "(%.2f, %.2f)", vec.x, vec.y);
        m_storableRecordValues[i->first] = value;
    }
    
    for (std::map<std::string, vector3*>::iterator i = m_storableObjectVector3Fields.begin(); i != m_storableObjectVector3Fields.end(); i++) {
        char value[50];
        vector3 vec = *(i->second);
        sprintf(value, "(%.2f, %.2f, %.2f)", vec.x, vec.y, vec.z);
        m_storableRecordValues[i->first] = value;
    }
    
    for (std::map<std::string, quaternion*>::iterator i = m_storableObjectQuaternionFields.begin(); i != m_storableObjectQuaternionFields.end(); i++) {
        char value[50];
        quaternion quat = *(i->second);
        sprintf(value, "(%.2f, %.2f, %.2f, %.2f)", quat.x, quat.y, quat.z, quat.w);
        m_storableRecordValues[i->first] = value;
    }
}

void StorableObject::UpdateStorableObjectFieldValue(std::string field, std::string value) {
    // Attempt to find the field type
    uint32_t type = 0;
    
	StorableObjectType* storableType = GetStorableObjectType();
    std::vector<StorableObjectField*> fields = storableType->GetFields();
    for (size_t i = 0; i < fields.size(); i++) {
        if (fields[i]->name == field) {
            type = fields[i]->type;
            break;
        }
    }
    
    assert(type > 0); // Field not found
    
    // Update the original source value (including type validation)
    switch (type) {
        case StorableObjectField::FIELD_RESOURCE:
			break;
        case StorableObjectField::FIELD_TEXT:
            *m_storableObjectStringFields[field] = value;
            break;
        case StorableObjectField::FIELD_FLOAT:
            *m_storableObjectFloatFields[field] = (float)atof(value.c_str());
            break;
        case StorableObjectField::FIELD_INT:
            *m_storableObjectIntFields[field] = atoi(value.c_str());
            break;
        case StorableObjectField::FIELD_VECTOR2:
            assert(sscanf(value.c_str(), "(%f, %f)", &m_storableObjectVector2Fields[field]->x, &m_storableObjectVector2Fields[field]->y) == 2); // Invalid value format
            break;
        case StorableObjectField::FIELD_VECTOR3:
            assert(sscanf(value.c_str(), "(%f, %f, %f)", &m_storableObjectVector3Fields[field]->x, &m_storableObjectVector3Fields[field]->y, &m_storableObjectVector3Fields[field]->z) == 3); // Invalid value format
            break;
        case StorableObjectField::FIELD_QUATERNION:
            assert(sscanf(value.c_str(), "(%f, %f, %f, %f)", &m_storableObjectQuaternionFields[field]->x, &m_storableObjectQuaternionFields[field]->y, &m_storableObjectQuaternionFields[field]->z, &m_storableObjectQuaternionFields[field]->w) == 4); // Invalid value format
            break;
        default:
            assert(false); // Field type undefined
            break;
    }
    
    // Save the value itself
    m_storableRecordValues[field] = value;
}

unsigned char* StorableObject::Serialize(int& size) {
    size = GetSerializedSize();
    
    // Loop over all fields and get total storage space required
	StorableObjectType* type = GetStorableObjectType();
    std::vector<StorableObjectField*> fields = type->GetFields();
        
    // Create some storage space
    unsigned char* buffer = (unsigned char*)malloc(size);
    MemoryWriter* writer = new MemoryWriter();
    writer->Initialize(buffer, size);

    // Loop over all fields again, writing data this time
    for (size_t i = 0; i < fields.size(); i++) {
        // Write the type
        uint32_t type = fields[i]->type;
        writer->Write(&type, sizeof(uint32_t));
        
        // Write field name length
        uint32_t length = fields[i]->name.length();
        writer->Write(&length, sizeof(uint32_t));
        
        // Write field name
		writer->Write((void*)fields[i]->name.data(), length + 1);
        
        // Write the data
        switch (fields[i]->type) {
            case StorableObjectField::FIELD_RESOURCE:
				// Write type
				length = m_storableObjectResourceFields[fields[i]->name]->GetGigaName().length();
				writer->Write(&length, sizeof(uint32_t));
				writer->Write((void*)m_storableObjectResourceFields[fields[i]->name]->GetGigaName().c_str(), length);
				
				// Write filename
				length = m_storableObjectResourceFields[fields[i]->name]->GetResource()->filename.length();
				writer->Write(&length, sizeof(uint32_t));
				writer->Write((void*)m_storableObjectResourceFields[fields[i]->name]->GetResource()->filename.c_str(), length);
				break;
            case StorableObjectField::FIELD_TEXT:
				length = m_storableObjectStringFields[fields[i]->name]->length();
                writer->Write(&length, sizeof(uint32_t));
                writer->Write((void*)m_storableObjectStringFields[fields[i]->name]->data(), length);
                break;
            case StorableObjectField::FIELD_FLOAT:
                writer->Write(m_storableObjectFloatFields[fields[i]->name], sizeof(uint32_t));
                break;
            case StorableObjectField::FIELD_INT:
                writer->Write(m_storableObjectIntFields[fields[i]->name], sizeof(uint32_t));
                break;
            case StorableObjectField::FIELD_VECTOR2:
                writer->Write(&m_storableObjectVector2Fields[fields[i]->name]->x, sizeof(uint32_t));
                writer->Write(&m_storableObjectVector2Fields[fields[i]->name]->y, sizeof(uint32_t));
                break;
            case StorableObjectField::FIELD_VECTOR3:
                writer->Write(&m_storableObjectVector3Fields[fields[i]->name]->x, sizeof(uint32_t));
                writer->Write(&m_storableObjectVector3Fields[fields[i]->name]->y, sizeof(uint32_t));
                writer->Write(&m_storableObjectVector3Fields[fields[i]->name]->z, sizeof(uint32_t));
                break;
            case StorableObjectField::FIELD_QUATERNION:
                writer->Write(&m_storableObjectQuaternionFields[fields[i]->name]->x, sizeof(uint32_t));
                writer->Write(&m_storableObjectQuaternionFields[fields[i]->name]->y, sizeof(uint32_t));
                writer->Write(&m_storableObjectQuaternionFields[fields[i]->name]->z, sizeof(uint32_t));
                writer->Write(&m_storableObjectQuaternionFields[fields[i]->name]->w, sizeof(uint32_t));
                break;
            default:
                assert(false); // Field type undefined
                break;
        }
    }
    
    delete writer;
    return(buffer);
}

int StorableObject::GetSerializedSize() {
    int size = 0;
    
    // Loop over all fields and get total storage space required
	StorableObjectType* type = GetStorableObjectType();
    std::vector<StorableObjectField*> fields = type->GetFields();
    for (size_t i = 0; i < fields.size(); i++) {
        // For all fields, make room for the type
        size += sizeof(uint32_t);
        
        // Then store the field name length + name (including null byte)
        size += sizeof(uint32_t);
        size += fields[i]->name.length() + 1;
        
        switch (fields[i]->type) {
            case StorableObjectField::FIELD_RESOURCE:
				size += sizeof(uint32_t); // Length of type string
				size += m_storableObjectResourceFields[fields[i]->name]->GetGigaName().length();
				size += sizeof(uint32_t);
				size += m_storableObjectResourceFields[fields[i]->name]->GetResource()->filename.length();
				break;
            case StorableObjectField::FIELD_TEXT:
                // For file and text types, store length as int32 then actual string
                size += sizeof(uint32_t) + m_storableObjectStringFields[fields[i]->name]->length();
                break;
            case StorableObjectField::FIELD_FLOAT:
                size += sizeof(uint32_t);
                break;
            case StorableObjectField::FIELD_INT:
                size += sizeof(uint32_t);
                break;
            case StorableObjectField::FIELD_VECTOR2:
                size += sizeof(uint32_t) * 2;
                break;
            case StorableObjectField::FIELD_VECTOR3:
                size += sizeof(uint32_t) * 3;
                break;
            case StorableObjectField::FIELD_QUATERNION:
                size += sizeof(uint32_t) * 4;
                break;
            default:
                assert(false); // Field type undefined
                break;
        }
    }
    
    return(size);
}

void StorableObject::Deserialize(MemoryReader* reader) {
	StorableObjectType* type = GetStorableObjectType();
	int fieldCount = type->GetFieldCount();
	int offset = 0;

    while(offset < fieldCount) {
        // Read in type
        uint32_t type = 0;
        reader->Read(&type, sizeof(uint32_t));
        
        // Read in field name length
        uint32_t length = 0;
        reader->Read(&length, sizeof(uint32_t));
        
        // Read in field name (including null byte)
        std::string name;
        name.resize(length + 1);
        reader->Read((void*)name.data(), length + 1);
		name.resize(length);
       
		// placeholders
		std::string p1;
		std::string p2;

		// Get resource system
		ResourceSystem* resourceSystem = GetSystem<ResourceSystem>();

        // Read in data
        switch (type) {
            case StorableObjectField::FIELD_RESOURCE:
				// Read type
				reader->Read(&length, sizeof(uint32_t));
				p1.resize(length);
				reader->Read((void*)p1.data(), length);

				// Read filename
				reader->Read(&length, sizeof(uint32_t));
				p2.resize(length);
				reader->Read((void*)p2.data(), length);

				// Load resource
				m_storableObjectResourceFields[name] = resourceSystem->LoadResource(p2, p1);
				break;
            case StorableObjectField::FIELD_TEXT:
                reader->Read(&length, sizeof(uint32_t));
				m_storableObjectStringFields[name]->resize(length);

                reader->Read((void*)m_storableObjectStringFields[name]->data(), length);
                break;
            case StorableObjectField::FIELD_FLOAT:
                reader->Read(m_storableObjectFloatFields[name], sizeof(uint32_t));
                break;
            case StorableObjectField::FIELD_INT:
                reader->Read(m_storableObjectIntFields[name], sizeof(uint32_t));
                break;
            case StorableObjectField::FIELD_VECTOR2:
                reader->Read(&m_storableObjectVector2Fields[name]->x, sizeof(uint32_t));
                reader->Read(&m_storableObjectVector2Fields[name]->y, sizeof(uint32_t));
                break;
            case StorableObjectField::FIELD_VECTOR3:
                reader->Read(&m_storableObjectVector3Fields[name]->x, sizeof(uint32_t));
                reader->Read(&m_storableObjectVector3Fields[name]->y, sizeof(uint32_t));
                reader->Read(&m_storableObjectVector3Fields[name]->z, sizeof(uint32_t));
                break;
            case StorableObjectField::FIELD_QUATERNION:
                reader->Read(&m_storableObjectQuaternionFields[name]->x, sizeof(uint32_t));
                reader->Read(&m_storableObjectQuaternionFields[name]->y, sizeof(uint32_t));
                reader->Read(&m_storableObjectQuaternionFields[name]->z, sizeof(uint32_t));
                reader->Read(&m_storableObjectQuaternionFields[name]->w, sizeof(uint32_t));
                break;
            default:
                assert(false); // Field type undefined
                break;
        }
        
		offset++;
    }
}

std::string StorableObject::GetStorableObjectFieldValue(std::string field) {
    // Attempt to find the field 
    std::string value = "";
    
    if (m_storableRecordValues.find(field) != m_storableRecordValues.end()) {
        value = m_storableRecordValues[field];
    }
    else {
		StorableObjectType* type = GetStorableObjectType();
        std::vector<StorableObjectField*> fields = type->GetFields();
        for (size_t i = 0; i < fields.size(); i++) {
            if (fields[i]->name == field) {
                value = fields[i]->defaultValue;
                break;
            }
        }
    }
    
    return(value);
}

StorableObjectType* StorableObject::GetStorableObjectType() {
	if (m_storableObjectType) {
		return(m_storableObjectType);
	}

	m_storableObjectType = DataLoader::GetRecordType(this->GetStorableTypeName());

	GIGA_ASSERT(m_storableObjectType != 0, "Storable object type not defined.");
	return(m_storableObjectType);
}