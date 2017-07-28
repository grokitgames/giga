
#ifndef animationtransform_hpp
#define animationtransform_hpp

/**
 * Transformation data for a single bone or sub-mesh
 */
struct GIGA_API AnimationTransform {
    /**
     * The object (mesh or bone) index we are working on
     */
    uint32_t objectID;
    
    /**
     * Transforms for scaling, rotation and translation
     */
    vector3 scaling;
    vector3 position;
    quaternion rotation;
};

#endif
