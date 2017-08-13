
#include <giga-engine.h>

Variant* Quaternion::GetX(std::string var, Variant* obj) {
    Quaternion* quat = obj->AsObject<Quaternion>();
    return(new Variant(quat->m_quat.x));
}

void Quaternion::SetX(std::string var, Variant* obj, Variant* val) {
    Quaternion* quat = obj->AsObject<Quaternion>();
    quat->m_quat.x = val->AsFloat();
}

Variant* Quaternion::GetY(std::string var, Variant* obj) {
    Quaternion* quat = obj->AsObject<Quaternion>();
    return(new Variant(quat->m_quat.y));
}

void Quaternion::SetY(std::string var, Variant* obj, Variant* val) {
    Quaternion* quat = obj->AsObject<Quaternion>();
    quat->m_quat.y = val->AsFloat();
}

Variant* Quaternion::GetZ(std::string var, Variant* obj) {
    Quaternion* quat = obj->AsObject<Quaternion>();
    return(new Variant(quat->m_quat.z));
}

void Quaternion::SetZ(std::string var, Variant* obj, Variant* val) {
    Quaternion* quat = obj->AsObject<Quaternion>();
    quat->m_quat.z = val->AsFloat();
}

Variant* Quaternion::GetW(std::string var, Variant* obj) {
    Quaternion* quat = obj->AsObject<Quaternion>();
    return(new Variant(quat->m_quat.w));
}

void Quaternion::SetW(std::string var, Variant* obj, Variant* val) {
    Quaternion* quat = obj->AsObject<Quaternion>();
    quat->m_quat.w = val->AsFloat();
}

void Quaternion::Initialize(Variant** argv, int argc) {
    if(argc == 0) {
        return;
    }
    
    GIGA_ASSERT(argc == 4, "Quaternion excepts 4 arguments.");
    
    for(int i = 0; i < argc; i++) {
        GIGA_ASSERT(argv[i]->IsFloat(), "Parameters to Quaternion should be floats.");
    }
    
    m_quat = quaternion(argv[3]->AsFloat(), argv[0]->AsFloat(), argv[1]->AsFloat(), argv[2]->AsFloat());
}
