
#ifndef command_hpp
#define command_hpp

class Command : public ScriptableObject {
public:
    Command() = default;
    ~Command() = default;
    
    GIGA_CLASS_NAME("Command");
    
protected:
    std::string m_name;
    uint32_t m_timestamp;
    uint32_t m_start;
};

#endif
