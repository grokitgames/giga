
#ifndef vector3_hpp
#define vector3_hpp

/**
 * Vector class, used mostly for scripting integration
 */
class GIGA_API Vector3 : public ScriptableObject {
public:
    Vector3() = default;
    Vector3(float x) : m_vector(x) { }
    Vector3(float x, float y, float z) : m_vector(x, y, z) { }
    Vector3(vector3 vec) : m_vector(vec) { }
    ~Vector3() = default;
    
    GIGA_CLASS_NAME("Vector3");
    
    /**
     * Get/set vector
     */
    vector3 Get() { return m_vector; }
    void Set(vector3 vec) { m_vector = vec; }
    
    /**
     * Scripting integration
     */
    static Variant* GetX(std::string var, Variant* obj);
    static void SetX(std::string var, Variant* obj, Variant* val);
    
    static Variant* GetY(std::string var, Variant* obj);
    static void SetY(std::string var, Variant* obj, Variant* val);
    
    static Variant* GetZ(std::string var, Variant* obj);
    static void SetZ(std::string var, Variant* obj, Variant* val);
    
    /**
     * Scripting constructor
     */
    void Initialize(Variant** argv, int argc);
    
protected:
    // Internal vector
    vector3 m_vector;
};

#endif 
