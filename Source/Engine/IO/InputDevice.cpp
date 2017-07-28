
#include <giga-engine.h>

bool InputDevice::GetButtonState(int button) {
    std::map<int, bool>::iterator i = m_buttons.find(button);
    if (i == m_buttons.end()) {
        return(false);
    }
    
    return(m_buttons[button]);
}

Variant* InputDevice::GetButtonState(Variant* object, int argc, Variant** argv) {
    GIGA_ASSERT(argc == 1, "GetButtonState expects one parameter");
    GIGA_ASSERT(argv[0]->IsInt(), "First parameter should be an integer button ID.");
    
    // Convert object to input device
    InputDevice* device = object->AsObject<InputDevice>();
    return(new Variant(device->GetButtonState(argv[0]->AsInt())));
}
