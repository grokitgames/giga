
#include <giga-engine.h>

InputSystem::~InputSystem() {
    for(size_t i = 0; i < m_mappings.size(); i++) {
        delete m_mappings[i];
    }
}

void InputSystem::Update(float delta) {
    for(size_t i = 0; i < m_devices.size(); i++) {
        m_devices[i]->Update();
    }
}

void InputSystem::RegisterInputDevice(InputDevice* device) {
    int type = device->GetType();
    for(size_t i = 0; i < m_devices.size(); i++) {
        if(m_devices[i]->GetType() == type) {
            GIGA_ASSERT(false, "There is already an input device of this type registered.");
        }
    }
    
    m_devices.push_back(device);
}

InputDevice* InputSystem::GetInputDevice(int type) {
    for(size_t i = 0; i < m_devices.size(); i++) {
        if(m_devices[i]->GetType() == type) {
            return(m_devices[i]);
        }
    }
    
    return(0);
}

void InputSystem::RegisterInputMapping(InputDevice* device, int button, std::string command) {
    // Do we already have something generating this command? If so, re-map
    for(size_t i = 0; i < m_mappings.size(); i++) {
        if(m_mappings[i]->command == command) {
            m_mappings[i]->device = device;
            m_mappings[i]->button = button;
            return;
        }
    }
    
    // Otherwise create a new mapping
    InputMapping* mapping = new InputMapping();
    mapping->device = device;
    mapping->button = button;
    mapping->command = command;
    
    m_mappings.push_back(mapping);
}

std::string InputSystem::FindInputMapping(InputDevice* device, int button) {
    std::string command;
    for(size_t i = 0; i < m_mappings.size(); i++) {
        if(m_mappings[i]->device == device && m_mappings[i]->button == button) {
            command = m_mappings[i]->command;
            break;
        }
    }
    
    return(command);
}

Variant* InputSystem::GetInputDevice(Variant* object, int argc, Variant** argv) {
    GIGA_ASSERT(argc == 1, "GetInputDevice expects one argument.");
    GIGA_ASSERT(argv[0]->IsInt(), "First parameter should be a device type int.");
    
    InputSystem* inputSystem = GetSystem<InputSystem>();
    return(new Variant(inputSystem->GetInputDevice(argv[0]->AsInt())));
}

Variant* InputSystem::RegisterInputMapping(Variant* object, int argc, Variant** argv) {
    GIGA_ASSERT(argc == 3, "RegisterInputMapping expects 3 arguments.");
    GIGA_ASSERT(argv[0]->IsObject(), "First parameter should be an InputDevice object.");
    GIGA_ASSERT(argv[1]->IsInt(), "Second parameter should be a key or button binding.");
    GIGA_ASSERT(argv[2]->IsString(), "Third parameter should be a command to generate");
    
    InputSystem* inputSystem = GetSystem<InputSystem>();
    inputSystem->RegisterInputMapping(argv[0]->AsObject<InputDevice>(), argv[1]->AsInt(), argv[2]->AsString());
    
    return(new Variant(0));
}

Variant* InputSystem::FindInputMapping(Variant* object, int argc, Variant** argv) {
    GIGA_ASSERT(argc == 3, "FindInputMapping expects 2 arguments.");
    GIGA_ASSERT(argv[0]->IsObject(), "First parameter should be an InputDevice object.");
    GIGA_ASSERT(argv[1]->IsInt(), "Second parameter should be a key or button binding.");
    
    InputSystem* inputSystem = GetSystem<InputSystem>();
    std::string command = inputSystem->FindInputMapping(argv[0]->AsObject<InputDevice>(), argv[1]->AsInt());
    return(new Variant(command));
}
