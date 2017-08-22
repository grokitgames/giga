
#include <giga-engine.h>

std::map<std::string, ComponentTemplateType*> ComponentTemplate::m_types;

void ComponentTemplate::RegisterComponentTemplateType(std::string name, ComponentTemplateType* type) {
    m_types[name] = type;
}

std::string ComponentTemplate::GetGigaName() {
    return(std::string(m_type->GetName()));
}

std::string ComponentTemplate::GetStorableTypeName() {
	return(std::string(m_type->GetName()));
}

Component* ComponentTemplate::CreateComponent(std::string name) {
    std::map<std::string, ComponentTemplateType*>::iterator i = m_types.find(name);
    GIGA_ASSERT(i != m_types.end(), "Unregisted type name");
    
    // Once we have a new component type, we need to set it up
    ComponentTemplate* ct = new ComponentTemplate();
    i->second->InitializeComponent(ct);
    return(ct);
}

void ComponentTemplate::RemoveComponent(Component* component) {
    
}

void ComponentTemplate::InitializeScriptableObject(std::string name) {
    SetupFromType(name);
}

void ComponentTemplate::InitializeStorableObject(std::string name) {
    SetupFromType(name);
}

void ComponentTemplate::SetupFromType(std::string name) {
    std::map<std::string, ComponentTemplateType*>::iterator i = m_types.find(name);
    GIGA_ASSERT(i != m_types.end(), "Unregisted type name");
    
    // Once we have a new component type, we need to set it up
    i->second->InitializeComponent(this);
}

Component* ComponentTemplate::Clone() {
    // Create
    ComponentTemplate* ct = new ComponentTemplate();
    m_type->InitializeComponent(ct);
    
    // Set fields
    std::map<std::string, int>::iterator i = m_fieldTypes.begin();
    for(i; i != m_fieldTypes.end(); i++) {
        int type = i->second;
        std::string name = i->first;
        
        // Add to lookup
        ct->m_fieldTypes[name] = type;
        
        // Set value based on type
        if(type == StorableObjectField::FIELD_INT) {
            ct->m_objectIntFields[name] = m_objectIntFields[name];
        }
        
        if(type == StorableObjectField::FIELD_TEXT) {
            ct->m_objectStringFields[name] = m_objectStringFields[name];
        }
        
        if(type == StorableObjectField::FIELD_FLOAT) {
            ct->m_objectFloatFields[name] = m_objectFloatFields[name];
        }
        
        if(type == StorableObjectField::FIELD_VECTOR2) {
            ct->m_objectVector2Fields[name] = m_objectVector2Fields[name];
        }
        
        if(type == StorableObjectField::FIELD_VECTOR3) {
            ct->m_objectVector3Fields[name] = m_objectVector3Fields[name];
        }
        
        if(type == StorableObjectField::FIELD_QUATERNION) {
            ct->m_objectQuaternionFields[name] = m_objectQuaternionFields[name];
        }
    }
    
    return(ct);
}

Variant* ComponentTemplate::Getter(std::string var, Variant* obj) {
    ComponentTemplate* t = obj->AsObject<ComponentTemplate>();
    return(new Variant(t->m_objectIntFields[var]));
}

void ComponentTemplate::Setter(std::string var, Variant* obj, Variant* val) {
    ComponentTemplate* t = obj->AsObject<ComponentTemplate>();
    
    // Switch on variable type
    switch(t->m_fieldTypes[var]) {
        case StorableObjectField::FIELD_INT:
            t->m_objectIntFields[var] = val->AsInt();
            break;
        case StorableObjectField::FIELD_TEXT:
            t->m_objectStringFields[var] = val->AsString();
            break;
        case StorableObjectField::FIELD_FLOAT:
            t->m_objectFloatFields[var] = val->AsFloat();
            break;
        case StorableObjectField::FIELD_VECTOR2:
            t->m_objectVector2Fields[var] = val->AsVector2();
            break;
        case StorableObjectField::FIELD_VECTOR3:
            t->m_objectVector3Fields[var] = val->AsVector3();
            break;
        case StorableObjectField::FIELD_QUATERNION:
            t->m_objectQuaternionFields[var] = val->AsQuaternion();
            break;
        default:
            break;
    }
}
