
#include <giga-engine.h>

Variant* Quaternion::GetX(Variant* obj) {
    Quaternion* quat = obj->AsObject<Quaternion>();
    return(new Variant(quat->m_quat.x));
}

void Quaternion::SetX(Variant* obj, Variant* val) {
    Quaternion* quat = obj->AsObject<Quaternion>();
    quat->m_quat.x = val->AsFloat();
}

Variant* Quaternion::GetY(Variant* obj) {
    Quaternion* quat = obj->AsObject<Quaternion>();
    return(new Variant(quat->m_quat.y));
}

void Quaternion::SetY(Variant* obj, Variant* val) {
    Quaternion* quat = obj->AsObject<Quaternion>();
    quat->m_quat.y = val->AsFloat();
}

Variant* Quaternion::GetZ(Variant* obj) {
    Quaternion* quat = obj->AsObject<Quaternion>();
    return(new Variant(quat->m_quat.z));
}

void Quaternion::SetZ(Variant* obj, Variant* val) {
    Quaternion* quat = obj->AsObject<Quaternion>();
    quat->m_quat.z = val->AsFloat();
}

Variant* Quaternion::GetW(Variant* obj) {
    Quaternion* quat = obj->AsObject<Quaternion>();
    return(new Variant(quat->m_quat.w));
}

void Quaternion::SetW(Variant* obj, Variant* val) {
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
