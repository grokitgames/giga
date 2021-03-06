
#ifndef variant_hpp
#define variant_hpp

class GIGA_API Variant {
public:
    virtual ~Variant();
    
    // Supported types
    enum Type {
        VAR_INT = 1,
        VAR_INT64,
        VAR_BOOL,
        VAR_FLOAT,
        VAR_VECTOR2,
        VAR_VECTOR3,
        VAR_VECTOR4,
        VAR_QUATERNION,
        VAR_STRING,
        VAR_OBJECT,
        VAR_ARRAY,
        VAR_LASTTYPE
    };
    
    struct Value {
        union {
			int32_t i;
            int64_t i64;
            bool b;
            float f1;
            char* str;
            void* ptr;
            GigaObject* obj;
        };
        
        union {
            float f2;
        };
        
        union {
            float f3;
        };
        
        union {
            float f4;
        };
    };
    
    /**
     * Constructors
     */
    Variant(int32_t value);
    Variant(int64_t value);
    Variant(bool value);
    Variant(float value);
    Variant(vector2 value);
    Variant(vector3 value);
    Variant(vector4 value);
    Variant(std::string value);
    Variant(GigaObject* value);
    Variant(const Variant& value);
    
    /**
     * Set operators
     */
    Variant& operator =(int32_t rhs);
    Variant& operator =(int64_t rhs);
    Variant& operator =(bool rhs);
    Variant& operator =(float rhs);
    Variant& operator =(vector2 rhs);
    Variant& operator =(vector3 rhs);
    Variant& operator =(vector4 rhs);
    Variant& operator =(quaternion rhs);
    Variant& operator =(std::string rhs);
    Variant& operator =(GigaObject* rhs);
    Variant& operator =(const Variant& rhs);
    
    /**
     * Checkers
     */
    bool IsInt();
    bool IsInt64();
    bool IsBool();
    bool IsFloat();
    bool IsNumeric();
    bool IsVector2();
    bool IsVector3();
    bool IsVector4();
    bool IsQuaternion();
    bool IsString();
    bool IsObject();
    
    /**
     * Convert value to string
     */
    std::string ToString();
    
    /**
     * Get operators
     */
    int AsInt();
    int64_t AsInt64();
    bool AsBool();
    float AsFloat();
    vector2 AsVector2();
    vector3 AsVector3();
    vector4 AsVector4();
    quaternion AsQuaternion();
    std::string AsString();
    GigaObject* AsObject();
    template<class T> T* AsObject() {
        if(m_type == VAR_OBJECT) {
            return(dynamic_cast<T*>(m_data.obj));
        }
        
        return(0);
    }
    
protected:
    // Make default constructor private so it needs to have a value
    Variant() = default;
    
protected:
    int m_type;
    Value m_data;
};

#endif
