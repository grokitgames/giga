
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
    static Variant* GetX(Variant* obj);
    static void SetX(Variant* obj, Variant* val);
    
    static Variant* GetY(Variant* obj);
    static void SetY(Variant* obj, Variant* val);
    
    static Variant* GetZ(Variant* obj);
    static void SetZ(Variant* obj, Variant* val);
    
    static Variant* GetW(Variant* obj);
    static void SetW(Variant* obj, Variant* val);
    
    /**
     * Scripting constructor
     */
    void Initialize(Variant** argv, int argc);
    
protected:
    // Internal quaternion
    quaternion m_quat;
};

#endif
