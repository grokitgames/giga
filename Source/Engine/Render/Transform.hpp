
#ifndef transform_hpp
#define transform_hpp

class GIGA_API Transform : public ScriptableObject {
public:
    Transform() : position(0, 0, 0), rotation(1, 0, 0, 0), scaling(1, 1, 1) { }
    ~Transform() = default;
    
    GIGA_CLASS_NAME("Transform");
    
    /**
     * Move/rotate/scale
     */
    void Move(vector3 translation);
    void Rotate(float degrees, vector3 axis);
    void Scale(vector3 scale);
    
    /**
     * Get model matrix
     */
    matrix4 GetModelMatrix();
    
    /**
     * Scripting integration
     */
    static Variant* GetPosition(std::string var, Variant* obj);
    static void SetPosition(std::string var, Variant* obj, Variant* val);
    
    static Variant* GetRotation(std::string var, Variant* obj);
    static void SetRotation(std::string var, Variant* obj, Variant* val);
    
    static Variant* GetScaling(std::string var, Variant* obj);
    static void SetScaling(std::string var, Variant* obj, Variant* val);
    
    static Variant* Translate(Variant* obj, int argc, Variant** argv);
    static Variant* Rotate(Variant* obj, int argc, Variant** argv);
    static Variant* Scale(Variant* obj, int argc, Variant** argv);
    
public:
    vector3 position;
    quaternion rotation;
    vector3 scaling;
    
protected:
    matrix4 m_modelMatrix;
    
private:
    void UpdateModelMatrix();
};

#endif 
