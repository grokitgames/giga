
#ifndef openglshadowpass_hpp
#define openglshadowpass_hpp

class OpenGLShadowPass : public ShadowPass {
public:
    OpenGLShadowPass() = default;
    ~OpenGLShadowPass() = default;
    
    /**
     * Initialize lighting pass
     */
    void Initialize(int width, int height);
    
    /**
     * Render
     */
    void Render(Scene* scene);
    
private:
    // Internally recurse through the children of a mesh component
    void RecursiveRender(StaticMeshComponent* mesh, matrix4 parent, Scene* scene, int index);
    
    // Storage for combined view/proj matrix
    matrix4 m_viewproj;
    matrix4 m_view;
    
    // Frustum
    Frustum* m_frustum;
    
    // Position of light
    vector3 m_position;
};

#endif
