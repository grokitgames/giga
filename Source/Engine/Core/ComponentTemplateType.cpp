
#include <giga-engine.h>

ComponentTemplateType::ComponentTemplateType() {
    m_storableType = 0;
    m_scriptableType = 0;
}

void ComponentTemplateType::SetName(std::string name) {
    GIGA_ASSERT(m_storableType == 0, "SetName should be called before any fields are added.");
    m_name = name;
    
    if(m_storableType) {
        delete m_storableType;
    }
    
    m_storableType = new StorableObjectType();
    m_storableType->SetName(name);
    
    if(m_scriptableType) {
        delete m_scriptableType;
    }
    
    m_scriptableType = new ScriptableObjectType(name);
}

void ComponentTemplateType::InitializeComponent(ComponentTemplate* ct) {
    // Set component template type
    ct->m_type = this;
    
    // Set fields
    std::map<std::string, int>::iterator i = m_objectVarTypes.begin();
    for(i; i != m_objectVarTypes.end(); i++) {
        int type = i->second;
        std::string name = i->first;
        
        // Add to lookup
        ct->m_fieldTypes[name] = type;
        
        // Set value based on type
        if(type == StorableObjectField::FIELD_INT) {
            ct->m_objectIntFields[name] = 0;
            ct->SetStorableObjectFieldMapping(name, &ct->m_objectIntFields[name]);
        }

        if(type == StorableObjectField::FIELD_TEXT) {
            ct->m_objectStringFields[name] = "";
            ct->SetStorableObjectFieldMapping(name, &ct->m_objectStringFields[name]);
        }
        
        if(type == StorableObjectField::FIELD_FLOAT) {
            ct->m_objectFloatFields[name] = 0.0f;
            ct->SetStorableObjectFieldMapping(name, &ct->m_objectFloatFields[name]);
        }
        
        if(type == StorableObjectField::FIELD_VECTOR2) {
            ct->m_objectVector2Fields[name] = vector2(0, 0);
            ct->SetStorableObjectFieldMapping(name, &ct->m_objectVector2Fields[name]);
        }
        
        if(type == StorableObjectField::FIELD_VECTOR3) {
            ct->m_objectVector3Fields[name] = vector3(0, 0, 0);
            ct->SetStorableObjectFieldMapping(name, &ct->m_objectVector3Fields[name]);
        }
        
        if(type == StorableObjectField::FIELD_QUATERNION) {
            ct->m_objectQuaternionFields[name] = quaternion(1, 0, 0, 0);
            ct->SetStorableObjectFieldMapping(name, &ct->m_objectQuaternionFields[name]);
        }
    }
}

void ComponentTemplateType::CreateField(std::string name, int type) {
    GIGA_ASSERT(m_name.length() > 0, "The SetName function must be called before AddField.");
    
    // Add type (for lookup later)
    m_objectVarTypes[name] = type;
    
    // Add our storable type
    m_storableType->AddField(name, type);
    
    // Create a callback function
    m_scriptableType->AddObjectVariable(name, &ComponentTemplate::Getter, &ComponentTemplate::Setter);
}

void ComponentTemplateType::Register(int typeID) {
    // Register with scripting system
    ScriptingSystem* scriptingSystem = GetSystem<ScriptingSystem>();
    scriptingSystem->RegisterScriptableObjectType<ComponentTemplate>(m_scriptableType);
    
    // Register with data loader
    DataLoader::RegisterRecordType<ComponentTemplate>(m_storableType);
    
    // Register as a component type
    Component::RegisterComponentType(m_name, typeID, &ComponentTemplate::CreateComponent, &ComponentTemplate::RemoveComponent);
    
    // Register as a template type
    ComponentTemplate::RegisterComponentTemplateType(m_name, this);
}

Variant* ComponentTemplateType::CreateField(Variant* obj, int argc, Variant** argv) {
    GIGA_ASSERT(argc == 2, "CreateField expects 2 arguments.");
    GIGA_ASSERT(argv[0]->IsString(), "First argument should be a string field name");
    GIGA_ASSERT(argv[1]->IsInt(), "Second argument should be a field type identifier");
    
    // Get the object out
    ComponentTemplateType* ct = obj->AsObject<ComponentTemplateType>();
    ct->CreateField(argv[0]->AsString(), argv[1]->AsInt());
    
    return(new Variant(0));
}

Variant* ComponentTemplateType::SetName(Variant* obj, int argc, Variant** argv) {
    GIGA_ASSERT(argc == 1, "SetName expects 1 argument.");
    GIGA_ASSERT(argv[0]->IsString(), "First argument should be a string name");
    
    // Get the object out
    ComponentTemplateType* ct = obj->AsObject<ComponentTemplateType>();
    ct->SetName(argv[0]->AsString());
    
    return(new Variant(0));
}

Variant* ComponentTemplateType::Register(Variant* obj, int argc, Variant** argv) {
    GIGA_ASSERT(argc == 1, "Register expects 1 argument.");
    GIGA_ASSERT(argv[0]->IsInt(), "First argument should be a unique component type identifier.");
    
    // Get the object out
    ComponentTemplateType* ct = obj->AsObject<ComponentTemplateType>();
    ct->Register(argv[0]->AsInt());
    
    return(new Variant(0));
}

