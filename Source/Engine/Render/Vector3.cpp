
#include <giga-engine.h>

Variant* Vector3::GetX(std::string var, Variant* obj) {
    Vector3* vec = obj->AsObject<Vector3>();
    return(new Variant(vec->m_vector.x));
}

void Vector3::SetX(std::string var, Variant* obj, Variant* val) {
    Vector3* vec = obj->AsObject<Vector3>();
    vec->m_vector.x = val->AsFloat();
}

Variant* Vector3::GetY(std::string var, Variant* obj) {
    Vector3* vec = obj->AsObject<Vector3>();
    return(new Variant(vec->m_vector.y));
}

void Vector3::SetY(std::string var, Variant* obj, Variant* val) {
    Vector3* vec = obj->AsObject<Vector3>();
    vec->m_vector.y = val->AsFloat();
}

Variant* Vector3::GetZ(std::string var, Variant* obj) {
    Vector3* vec = obj->AsObject<Vector3>();
    return(new Variant(vec->m_vector.z));
}

void Vector3::SetZ(std::string var, Variant* obj, Variant* val) {
    Vector3* vec = obj->AsObject<Vector3>();
    vec->m_vector.z = val->AsFloat();
}

void Vector3::Initialize(Variant** argv, int argc) {
    if(argc == 0) {
        return;
    }
    
    GIGA_ASSERT(argc == 1 || argc == 3, "Vector3 excepts 0, 1 or 3 arguments.");
    
    for(int i = 0; i < argc; i++) {
        GIGA_ASSERT(argv[i]->IsNumeric(), "Parameters to Vector3 should be floats.");
    }
    
    if(argc == 1) {
        m_vector = vector3(argv[0]->AsFloat());
    }
    else {
        m_vector = vector3(argv[0]->AsFloat(), argv[1]->AsFloat(), argv[2]->AsFloat());
    }
}
