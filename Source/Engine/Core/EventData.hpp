
#ifndef eventdata_h
#define eventdata_h

/**
 * Generic base class for event data to attach to events
 */
class GIGA_API EventData : public ScriptableObject {
public:
    EventData() = default;
    virtual ~EventData() = default;
};

#endif
