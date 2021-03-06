
#include <giga-engine.h>

Variant::~Variant() {
    if(m_type == VAR_STRING) {
        free(m_data.str);
    }
}

Variant::Variant(int32_t value) {
    *this = value;
}

Variant::Variant(int64_t value){
    *this = value;
}

Variant::Variant(bool value) {
    *this = value;
}

Variant::Variant(float value) {
    *this = value;
}

Variant::Variant(vector2 value) {
    *this = value;
}

Variant::Variant(vector3 value) {
    *this = value;
}

Variant::Variant(vector4 value) {
    *this = value;
}

Variant::Variant(std::string value) {
    *this = value;
}

Variant::Variant(GigaObject* value) {
    *this = value;
}

Variant::Variant(const Variant& value) {
    *this = value;
}

Variant& Variant::operator =(int32_t rhs) {
    m_type = VAR_INT;
    m_data.i = rhs;
    return *this;
}

Variant& Variant::operator =(int64_t rhs) {
    m_type = VAR_INT64;
    m_data.i64 = rhs;
    return *this;
}

Variant& Variant::operator =(bool rhs) {
    m_type = VAR_BOOL;
    m_data.b = rhs;
    return *this;
}

Variant& Variant::operator =(float rhs) {
    m_type = VAR_FLOAT;
    m_data.f1 = rhs;
    return *this;
}

Variant& Variant::operator =(vector2 rhs) {
    m_type = VAR_VECTOR2;
    m_data.f1 = rhs.x;
    m_data.f2 = rhs.y;
    return *this;
}

Variant& Variant::operator =(vector3 rhs) {
    m_type = VAR_VECTOR3;
    m_data.f1 = rhs.x;
    m_data.f2 = rhs.y;
    m_data.f3 = rhs.z;
    return *this;
}

Variant& Variant::operator =(vector4 rhs) {
    m_type = VAR_VECTOR4;
    m_data.f1 = rhs.x;
    m_data.f2 = rhs.y;
    m_data.f3 = rhs.z;
    m_data.f4 = rhs.w;
    return *this;
}

Variant& Variant::operator =(quaternion rhs) {
    m_type = VAR_QUATERNION;
    m_data.f1 = rhs.x;
    m_data.f2 = rhs.y;
    m_data.f3 = rhs.z;
    m_data.f4 = rhs.w;
    return *this;
}

Variant& Variant::operator =(std::string rhs) {
    m_type = VAR_STRING;
    m_data.str = (char*)malloc(rhs.length() + 1);
    strcpy(m_data.str, rhs.data());
    return *this;
}

Variant& Variant::operator =(GigaObject* rhs) {
    m_type = VAR_OBJECT;
    m_data.obj = rhs;
    return *this;
}

Variant& Variant::operator =(const Variant& rhs) {
    m_type = rhs.m_type;
    m_data = rhs.m_data;
    return *this;
}

bool Variant::IsInt() {
    return(m_type == VAR_INT);
}

bool Variant::IsInt64() {
    return(m_type == VAR_INT64);
}

bool Variant::IsBool() {
    return(m_type == VAR_BOOL);
}

bool Variant::IsFloat() {
    return(m_type == VAR_FLOAT);
}

bool Variant::IsNumeric() {
    return(m_type == VAR_INT || m_type == VAR_FLOAT);
}

bool Variant::IsVector2() {
    return(m_type == VAR_VECTOR2);
}

bool Variant::IsVector3() {
    return(m_type == VAR_VECTOR3);
}

bool Variant::IsVector4() {
    return(m_type == VAR_VECTOR4);
}

bool Variant::IsQuaternion() {
    return(m_type == VAR_QUATERNION);
}

bool Variant::IsString() {
    return(m_type == VAR_STRING);
}

bool Variant::IsObject() {
    return(m_type == VAR_OBJECT);
}

std::string Variant::ToString() {
    std::string output;
    switch(m_type) {
        case VAR_INT:
            output = std::to_string(m_data.i);
            break;
        case VAR_BOOL:
            output = m_data.b ? "true" : "false";
            break;
        case VAR_FLOAT:
            output = std::to_string(m_data.f1);
            break;
        case VAR_VECTOR2:
            output.resize(50);
            sprintf((char*)output.data(), "(%.2f, %.2f)", m_data.f1, m_data.f2);
            break;
        case VAR_VECTOR3:
            output.resize(50);
            sprintf((char*)output.data(), "(%.2f, %.2f, %.2f)", m_data.f1, m_data.f2, m_data.f3);
        case VAR_VECTOR4:
            output.resize(50);
            sprintf((char*)output.data(), "(%.2f, %.2f, %.2f, %.2f)", m_data.f1, m_data.f2, m_data.f3, m_data.f4);
        case VAR_QUATERNION:
            output.resize(50);
            sprintf((char*)output.data(), "(%.2f, %.2f, %.2f, %.2f)", m_data.f4, m_data.f2, m_data.f3, m_data.f1);
        case VAR_STRING:
            output = m_data.str;
            break;
        case VAR_OBJECT:
            output = m_data.obj->ToString();
        default:
            break;
    }
    
    return(output);
}

int Variant::AsInt() {
    if(m_type == VAR_INT) {
        return m_data.i;
    }
    
    if(m_type == VAR_FLOAT) {
        return((int)m_data.f1);
    }
    
    return(0);
}

int64_t Variant::AsInt64() {
    if(m_type == VAR_INT64) {
        return m_data.i64;
    }
    
    if(m_type == VAR_INT) {
        return((int64_t)m_data.i);
    }
    
    return(0);
}

bool Variant::AsBool() {
    if(m_type == VAR_BOOL) {
        return(m_data.b);
    }
    
    return(false);
}

float Variant::AsFloat() {
    if(m_type == VAR_FLOAT) {
        return(m_data.f1);
    }
    
    if(m_type == VAR_INT) {
        return((float)m_data.i);
    }
    
    return(0.0f);
}

vector2 Variant::AsVector2() {
    if(m_type == VAR_VECTOR2) {
        return vector2(m_data.f1, m_data.f2);
    }
    
    return(vector2(0, 0));
}

vector3 Variant::AsVector3() {
    if(m_type == VAR_VECTOR3) {
        return vector3(m_data.f1, m_data.f2, m_data.f3);
    }
    
    return(vector3(0, 0, 0));
}

vector4 Variant::AsVector4() {
    if(m_type == VAR_VECTOR4) {
        return vector4(m_data.f1, m_data.f2, m_data.f3, m_data.f4);
    }
    
    return(vector4(0, 0, 0, 0));
}

quaternion Variant::AsQuaternion() {
    if(m_type == VAR_QUATERNION) {
        return quaternion(m_data.f4, m_data.f2, m_data.f3, m_data.f1);
    }
    
    return(quaternion(1, 0, 0, 0));
}

std::string Variant::AsString() {
    if(m_type == VAR_STRING) {
        return(m_data.str);
    }
    
    return(std::string(""));
}

GigaObject* Variant::AsObject() {
    if(m_type == VAR_OBJECT) {
        return m_data.obj;
    }
    
    return(0);
}

