
#include <giga-engine.h>

void Plane::CreateFromPoints(vector3& p0, vector3& p1, vector3 &p2) {
    vector3 v1 = p1 - p0;
    vector3 v2 = p2 - p0;
    
    abc = glm::normalize(glm::cross(v1, v2));
    
    d = -glm::dot(p0, abc);
}

float Plane::Distance(vector3& point) {
    return(abc.x * point.x + abc.y * point.y + abc.z * point.z + d);
}
