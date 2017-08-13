
#ifndef keyboard_hpp
#define keyboard_hpp

/**
 * Keyboard event data
 */
class GIGA_API KeyboardEventData : public EventData {
public:
    KeyboardEventData() = default;
    KeyboardEventData(int key) : m_key(key) { }
    
    GIGA_CLASS_NAME("KeyboardEventData")
    
    /**
     * Scripting integration
     */
    static Variant* GetKey(std::string name, Variant* object);
    
protected:
    // Key that was pressed or released
    int m_key;
};

/**
 * Keyboard input device
 */
class GIGA_API Keyboard : public InputDevice {
public:
    Keyboard();
    ~Keyboard() = default;
    
    GIGA_CLASS_NAME("Keyboard")
    
    /**
     * Initialize our input device
     */
    void Initialize();
    
    /**
     * Handle a keyboard action (specific callback for our platform library)
     */
    void ProcessAction(int key, int action);
};

#endif
