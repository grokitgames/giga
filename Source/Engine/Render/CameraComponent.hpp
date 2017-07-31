
#ifndef cameracomponent_hpp
#define cameracomponent_hpp

class GIGA_API CameraComponent : public RenderComponent {
public:
    CameraComponent();
    ~CameraComponent() = default;
    
    GIGA_CLASS_NAME("CameraComponent");
    
    /**
     * Set the look vector
     */
    void SetLookVector(vector3 point);
    
    /**
     * Get vectors
     */
    vector3 GetLookVector() { return m_look; }
    vector3 GetUpVector() { return m_up; }
    vector3 GetRightVector() { return m_right; }
    
    /**
     * Get the camera view frustum
     */
    Frustum GetViewFrustum() { return m_viewFrustum; }
    
    /**
     * Get matrices
     */
    matrix4 GetViewMatrix() { return m_view; }
    matrix4 GetProjectionMatrix() { return m_projection; }
    
    /**
     * Get near/far values
     */
    float GetNear() { return m_fnear; }
    float GetFar() { return m_ffar; }
    
    /**
     * Set size of window viewport
     */
    void SetViewport(int width, int height);
    
    /**
     * Get aspect ratio and FOV
     */
    float GetAspectRatio() { return m_aspect; }
    void SetAspectRatio(float m_ratio);
    float GetFOV() { return m_fov; }
    
    /**
     * Calculate a frustum with a near and far set of numbers with our aspect ratio and fov
     */
    Frustum CalculateFrustum(float near, float far);
    
    /**
     * Clone this camera
     */
    CameraComponent* Clone();
    
    /**
     * Scripting integration
     */
    static Variant* GetFOV(Variant* obj);
    static void SetFOV(Variant* obj, Variant* val);
    
    static Variant* SetViewport(Variant* obj, int argc, Variant** argv);
    
    static Variant* GetNear(Variant* obj);
    static void SetNear(Variant* obj, Variant* val);
    
    static Variant* GetFar(Variant* obj);
    static void SetFar(Variant* obj, Variant* val);
    
    static Variant* GetLook(Variant* obj);
    static void SetLook(Variant* obj, Variant* val);
    
    static Variant* GetRight(Variant* obj);
    static Variant* GetUp(Variant* obj);
    
protected:
    /**
     * Update view matrices and frustums
     */
    virtual void CalculateMatricesAndFrustum();
    
protected:
    // Our field of view and aspect ratio
    float m_fov, m_aspect;
    
    // Our near and far distances
    float m_fnear, m_ffar;
    
    // Width and height of our camera viewport
    int m_width, m_height;
    
    // Look vector
    vector3 m_look;
    
    // Up vector
    vector3 m_up;
    
    // Right vector
    vector3 m_right;
    
    // Viewing frustum
    Frustum m_viewFrustum;
    
    // View matrix
    matrix4 m_view;
    
    // Projection matrix
    matrix4 m_projection;
};

#endif
