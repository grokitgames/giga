
#ifndef shadersystem_hpp
#define shadersystem_hpp

/**
 * Behind the scenes shader management system for creating and managing shader variations
 */
class GIGA_API ShaderSystem {
public:
    ShaderSystem() = default;
    ~ShaderSystem();
    
    struct CachedVariation {
        ShaderVariation* sv;
        std::string vert;
        std::string frag;
        std::vector<std::string> defines;
    };
    
    /**
     * Find an existing shader variation based on vertex/fragment shader and definitions
     */
    static ShaderVariation* Find(std::string vert, std::string frag, ShaderAttributes* attribs);
    
protected:
    // A list of shader variations already in use
    static std::vector<CachedVariation*> m_cache;
};

#endif
