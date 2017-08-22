
#ifndef texture3d_hpp
#define texture3d_hpp

/**
 * Our class for storing 3D (cubemap) textures
 */
class GIGA_API Texture3D : public Texture {
public:
    Texture3D() = default;
    virtual ~Texture3D() = default;

	GIGA_CLASS_NAME("Texture3D");
};

#endif
