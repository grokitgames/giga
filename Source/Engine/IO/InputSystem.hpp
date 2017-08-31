
#ifndef inputsystem_hpp
#define inputsystem_hpp

class GIGA_API InputSystem : public System {
public:
    InputSystem() = default;
    ~InputSystem();
    
	GIGA_CLASS_NAME("InputSystem");

	/**
	 * Initialize (scripting interfaces)
	 */
	void Initialize();
    
    /**
     * Update input devices (polling mostly)
     */
    void Update(float delta);
    
    /**
     * Register a new input device (mouse, keyboard, joystick)
     */
    void RegisterInputDevice(InputDevice* device);
    
    /**
     * Get a device by device type
     */
    InputDevice* GetInputDevice(int type);
    
    /**
     * Set a binding for a command event to be created
     */
    void RegisterInputMapping(InputDevice* device, int button, std::string command);
    
    /**
     * Find an input mapping
     */
    std::string FindInputMapping(InputDevice* device, int button);
    
    /**
     * Scripting integration
     */
    static Variant* GetInputDevice(Variant* object, int argc, Variant** argv);
    static Variant* RegisterInputMapping(Variant* object, int argc, Variant** argv);
    static Variant* FindInputMapping(Variant* object, int argc, Variant** argv);
    
private:
    struct InputMapping {
        int button;
        InputDevice* device;
        std::string command;
    };
    
protected:
    // List of registered input devices (max one per type)
    std::vector<InputDevice*> m_devices;
    
    // List of registered input mappings
    std::vector<InputMapping*> m_mappings;

	// History of commands
	std::vector<Command*> m_commands;
};

#endif
