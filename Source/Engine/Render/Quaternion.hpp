
#ifndef quaternion_hpp
#define quaternion_hpp

/**
 * Quaternion class, used mostly for scripting integration
 */
class GIGA_API Quaternion : public ScriptableObject {
public:
    Quaternion() = default;
    Quaternion(float x, float y, float z, float w) : m_quat(w, x, y, z) { }
    Quaternion(quaternion quat) : m_quat(quat) { }
    ~Quaternion() = default;
    
    GIGA_CLASS_NAME("Quaternion");
    
    /**
     * Get/set vector
     */
    quaternion Get() { return m_quat; }
    void Set(quaternion quat) { m_quat = quat; }
    
    /**
     * Scripting integration
     */
    static Variant* GetX(std::string var, Variant* obj);
    static void SetX(std::string var, Variant* obj, Variant* val);
    
    static Variant* GetY(std::string var, Variant* obj);
    static void SetY(std::string var, Variant* obj, Variant* val);
    
    static Variant* GetZ(std::string var, Variant* obj);
    static void SetZ(std::string var, Variant* obj, Variant* val);
    
    static Variant* GetW(std::string var, Variant* obj);
    static void SetW(std::string var, Variant* obj, Variant* val);
    
    /**
     * Scripting constructor
     */
    void Initialize(Variant** argv, int argc);
    
protected:
    // Internal quaternion
    quaternion m_quat;
};

#endif
