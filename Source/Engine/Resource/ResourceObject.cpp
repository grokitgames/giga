
#include <giga-engine.h>

ResourceObject::ResourceObject() {
    m_resource = 0;
}

ResourceObject::~ResourceObject() {
    if(m_resource) {
        delete m_resource;
    }
}

unsigned char* ResourceObject::GetData() {
    assert(m_resource != 0);
    return(m_resource->GetData());
}

std::string ResourceObject::GetString() {
    assert(m_resource != 0);
    
    unsigned char* data = m_resource->GetData();
    int length = m_resource->GetFileSize();
    
	std::string retval = "";
	if (data) {
		retval += (char*)data;
	}
    retval.resize(length);
    return(retval);
}
