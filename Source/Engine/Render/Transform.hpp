
#ifndef transform_hpp
#define transform_hpp

class Vector3;
class Quaternion;

class GIGA_API Transform : public ScriptableObject {
public:
	Transform();
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
	 * Set component
	 */
	void SetComponent(Component* component) { m_component = component; }
    
    /**
     * Scripting integration
     */
    static Variant* GetPosition(std::string var, Variant* obj);
    static void SetPosition(std::string var, Variant* obj, Variant* val);
    
    static Variant* GetRotation(std::string var, Variant* obj);
    static void SetRotation(std::string var, Variant* obj, Variant* val);
    
    static Variant* GetScaling(std::string var, Variant* obj);
    static void SetScaling(std::string var, Variant* obj, Variant* val);
    
    static Variant* GetLook(std::string var, Variant* obj);
    static Variant* GetUp(std::string var, Variant* obj);
    static Variant* GetRight(std::string var, Variant* obj);
    
    static Variant* Translate(Variant* obj, int argc, Variant** argv);
    static Variant* Rotate(Variant* obj, int argc, Variant** argv);
    static Variant* Scale(Variant* obj, int argc, Variant** argv);
    
public:
    vector3 position;
    quaternion rotation;
    vector3 scaling;
    
    vector3 look;
    vector3 up;
    vector3 right;

	// Scripting variables
	Vector3* vlook;
	Vector3* vup;
	Vector3* vright;

	Vector3* vposition;
	Vector3* vrotation;
	Vector3* vscaling;
    
protected:
    matrix4 m_modelMatrix;

	// Component this transform is attached to (if applicable)
	Component* m_component;
    
private:
    void UpdateModelMatrix();
};

#endif 
