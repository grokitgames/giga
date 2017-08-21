
#include <giga-engine.h>

std::vector<StorableObjectType*> DataLoader::m_recordTypes;

std::vector<StorableObject*> DataLoader::GetRecords(StorableObjectType* type, int sceneID) {
    std::vector<StorableObject*> vec;
    return(vec);
}

StorableObjectType* DataLoader::GetRecordType(std::string type) {
    for (int i = 0; i < m_recordTypes.size(); i++) {
        if (m_recordTypes[i]->GetName() == type) {
            return(m_recordTypes[i]);
        }
    }
    
	GIGA_ASSERT(false, "Type not found");
    return(0);
}
