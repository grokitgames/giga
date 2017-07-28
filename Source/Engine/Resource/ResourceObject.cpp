
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
