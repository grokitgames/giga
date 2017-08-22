
#ifndef shader_hpp
#define shader_hpp

/**
 * Base shader class for loading individual shader files
 */
class Shader : public ResourceObject {
public:
    Shader() = default;
    ~Shader() = default;

	GIGA_CLASS_NAME("Shader");
};

#endif 
