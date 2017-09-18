
#ifndef storableobjectfield_h
#define storableobjectfield_h

class GIGA_API StorableObjectField {
public:
    StorableObjectField() : editable(false), type(0) { }
    ~StorableObjectField() { }
    
    std::string name;
    std::string friendly_name;
    bool editable;
    int type;
    std::string defaultValue;
    
    enum {
        FIELD_TEXT = 1,
        FIELD_INT,
        FIELD_FLOAT,
        FIELD_RESOURCE,
        FIELD_VECTOR2,
        FIELD_VECTOR3,
        FIELD_QUATERNION,
		FIELD_FOREIGNKEY
    };
};

#endif
