
#include <giga-engine.h>

int Frustum::Intersects(vector3& point) {
    for (int i = 0; i < 6; i++) {
        if (planes[i].Distance(point) > 0)
            return(0);
    }
    return(1);
}

int Frustum::Intersects(BoundingBox &box) {
    for (int p = 0; p < 6; p++) {
        if (planes[p].Distance(box.points[0]) < 0)
            continue;
        if (planes[p].Distance(box.points[1]) < 0)
            continue;
        if (planes[p].Distance(box.points[2]) < 0)
            continue;
        if (planes[p].Distance(box.points[3]) < 0)
            continue;
        if (planes[p].Distance(box.points[4]) < 0)
            continue;
        if (planes[p].Distance(box.points[5]) < 0)
            continue;
        if (planes[p].Distance(box.points[6]) < 0)
            continue;
        if (planes[p].Distance(box.points[7]) < 0)
            continue;
        return(0);
    }
    return(1);
}
