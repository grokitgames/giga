
#ifndef shadervariation_hpp
#define shadervariation_hpp

/**
 * A list of shader variations/definitions
 */
class ShaderAttributes {
public:
    ShaderAttributes() = default;
    ~ShaderAttributes() = default;
    
    void Add(std::string define) { definitions[define] = true; }
    
    ShaderAttributes(const ShaderAttributes &rhs) {
        this->definitions = rhs.definitions;
    }
    
    ShaderAttributes& operator = (const ShaderAttributes &rhs) {
        this->definitions = rhs.definitions;
        return(*this);
    }
    
public:
    std::map<std::string, bool> definitions;
};

/**
 * A single variation of definitions
 */
class ShaderVariation {
public:
    ShaderVariation();
    ~ShaderVariation();
    
    /**
     * A variable inside the shader program (to cache its location)
     */
    struct ShaderVariable {
        std::string name;
        int location;
        bool uniform;
    };
    
    /**
     * Set vertex/fragment shaders
     */
    void SetVertexShader(Shader* shader) { m_vertexShader = shader; }
    void SetFragmentShader(Shader* shader) { m_fragmentShader = shader; }
    
    /**
     * Add a definition
     */
    void Define(std::string define);
    
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
    
    /**
     * Get shader definitions
     */
    ShaderAttributes* GetShaderAttributes() { return m_attribs; }
    
private:
    /**
     * Interate over our internal list of shader vars
     */
    int _var(std::string name, bool uniform);
    
protected:
    // Definitions that make up the content
    ShaderAttributes* m_attribs;
    
    // Vertex shader
    Shader* m_vertexShader;
    
    // Fragment shader
    Shader* m_fragmentShader;
    
    // Internal OpenGL program and shader IDs
    unsigned int m_program;
    unsigned int m_vshader;
    unsigned int m_fshader;
    
    // Cache of shader locations
    std::vector<ShaderVariable*> m_vars;
};

#endif
