
#ifndef openglshadervariation_hpp
#define openglshadervariation_hpp

/**
 * A single variation of definitions
 */
class OpenGLShaderVariation : public ShaderVariation {
public:
    OpenGLShaderVariation();
    ~OpenGLShaderVariation();
    
    /**
     * Bind/unbind this for use
     */
    void Bind();
    void Unbind();
    
    /**
     * Get the location of a uniform variable
     */
    int Uniform(std::string name);
    
    /**
     * Get the location of an attribute
     */
    int Attribute(std::string name);
    
    /**
     * Variable setting functions
     */
    void Set(std::string name, float value);
    void Set(std::string name, vector2 value);
    void Set(std::string name, vector3 value);
    void Set(std::string name, vector4 value);
    void Set(std::string name, int value);
    void Set(std::string name, unsigned int value);
    void Set(std::string name, matrix4 value);

private:
    /**
     * Interate over our internal list of shader vars
     */
    int _var(std::string name, bool uniform);
    
protected:
    // Internal OpenGL program and shader IDs
    unsigned int m_program;
    unsigned int m_vshader;
    unsigned int m_fshader;
};

#endif
