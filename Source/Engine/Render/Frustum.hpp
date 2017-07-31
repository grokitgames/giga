
#ifndef frustum_hpp
#define frustum_hpp

/**
 * Define our viewing frustum for frustum culling
 */
class GIGA_API Frustum {
public:
    Frustum() = default;
    ~Frustum() = default;
    
    enum {
        COORD_BOTTOMLEFT = 0,
        COORD_BOTTOMRIGHT,
        COORD_TOPLEFT,
        COORD_TOPRIGHT
    };
    
    enum {
        PLANE_LEFT = 0,
        PLANE_RIGHT,
        PLANE_BOTTOM,
        PLANE_TOP,
        PLANE_NEAR,
        PLANE_FAR
    };
    
    /**
     * Check a single point
     */
    int Intersects(vector3& point);
    
    /**
     * Check a bounding box
     */
    int Intersects(BoundingBox& box);
    
public:
    /**
     * Defines our near and far vertices
     */
    vector3 fnear[4];
    vector3 ffar[4];
    
    /**
     * The center of the frustum (useful for extents/CSM)
     */
    vector3 center;
    
    /**
     * Planes
     */
    Plane planes[6];
};

#endif
