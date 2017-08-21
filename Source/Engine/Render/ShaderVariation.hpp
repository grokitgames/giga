
#ifndef shadervariation_hpp
#define shadervariation_hpp

/**
 * A list of shader variations/definitions
 */
class GIGA_API ShaderAttributes {
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
class GIGA_API ShaderVariation {
public:
    ShaderVariation();
    virtual ~ShaderVariation();
    
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
	Shader* GetVertexShader() { return m_vertexShader; }

    void SetFragmentShader(Shader* shader) { m_fragmentShader = shader; }
	Shader* GetFragmentShader() { return m_fragmentShader; }
    
    /**
     * Add a definition
     */
    void Define(std::string define);
    
    /**
     * Bind/unbind this for use
     */
    virtual void Bind() { }
    virtual void Unbind() { }
    
    /**
     * Get the location of a uniform variable
     */
    virtual int Uniform(std::string name) { assert(false); return(0); }
    
    /**
     * Get the location of an attribute
     */
    virtual int Attribute(std::string name) { assert(false); return(0); }
    
    /**
     * Variable setting functions
     */
    virtual void Set(std::string name, float value) { }
    virtual void Set(std::string name, vector2 value) { }
    virtual void Set(std::string name, vector3 value) { }
    virtual void Set(std::string name, vector4 value) { }
    virtual void Set(std::string name, int value) { }
    virtual void Set(std::string name, unsigned int value)  { }
    virtual void Set(std::string name, matrix4 value) { }
    
    /**
     * Get shader definitions
     */
    ShaderAttributes* GetShaderAttributes() { return m_attribs; }
    
protected:
    // Definitions that make up the content
    ShaderAttributes* m_attribs;
    
    // Vertex shader
    Shader* m_vertexShader;
    
    // Fragment shader
    Shader* m_fragmentShader;
    
    // Cache of shader locations
    std::vector<ShaderVariable*> m_vars;
};

#endif
