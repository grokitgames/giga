
#include <giga-engine.h>

void Transform::Move(vector3 translation) {
    position += translation;
}

void Transform::Rotate(float degrees, vector3 axis) {
    quaternion delta = glm::angleAxis(glm::radians(degrees), axis);
    rotation = rotation * delta;;
}

void Transform::Scale(vector3 scale) {
    scaling *= scale;
}

matrix4 Transform::GetModelMatrix() {
    UpdateModelMatrix();
    return(m_modelMatrix);
}

void Transform::UpdateModelMatrix() {
    matrix4 t = glm::translate(glm::mat4(1.0), position);
    matrix4 s = glm::scale(glm::mat4(1.0), scaling);
    matrix4 r = glm::mat4_cast(rotation);
    
    m_modelMatrix = t * r * s;
}

Variant* Transform::GetPosition(Variant* obj) {
    Transform* tc = obj->AsObject<Transform>();
    Vector3* sv = new Vector3(tc->position);
    
    return(new Variant(sv));
}

void Transform::SetPosition(Variant* obj, Variant* val) {
    Transform* tc = obj->AsObject<Transform>();
    Vector3* sv = val->AsObject<Vector3>();
    tc->position = sv->Get();
}

Variant* Transform::GetRotation(Variant* obj) {
    Transform* tc = obj->AsObject<Transform>();
    Quaternion* sq = new Quaternion(tc->rotation);
    return(new Variant(sq));
}

void Transform::SetRotation(Variant* obj, Variant* val) {
    Transform* tc = obj->AsObject<Transform>();
    Quaternion* sq = val->AsObject<Quaternion>();
    tc->rotation = sq->Get();
}

Variant* Transform::GetScaling(Variant* obj) {
    Transform* tc = obj->AsObject<Transform>();
    Vector3* sv = new Vector3(tc->scaling);
    return(new Variant(sv));
}

void Transform::SetScaling(Variant* obj, Variant* val) {
    Transform* tc = obj->AsObject<Transform>();
    Vector3* sv = val->AsObject<Vector3>();
    tc->scaling = sv->Get();
}

Variant* Transform::Translate(Variant* obj, int argc, Variant** argv) {
    GIGA_ASSERT(argc == 1, "Expected 1 argument.");
    GIGA_ASSERT(argv[0]->IsObject(), "Expected vector as argument.");
    
    Transform* tc = obj->AsObject<Transform>();
    Vector3* sv = argv[0]->AsObject<Vector3>();
    tc->Move(sv->Get());
    
    sv->Set(tc->position);
    return(new Variant(sv));
}

Variant* Transform::Rotate(Variant* obj, int argc, Variant** argv) {
    GIGA_ASSERT(argc == 2, "Expected 2 arguments.");
    GIGA_ASSERT(argv[0]->IsNumeric(), "Expected float as 1st argument.");
    GIGA_ASSERT(argv[1]->IsObject(), "Expected vector as 2nd argument.");
    
    Transform* tc = obj->AsObject<Transform>();
    Vector3* axis = argv[1]->AsObject<Vector3>();
    float angle = argv[0]->AsFloat();
    
    tc->Rotate(angle, axis->Get());
    
    Quaternion* sq = new Quaternion();
    sq->Set(tc->rotation);
    return(new Variant(sq));
}

Variant* Transform::Scale(Variant* obj, int argc, Variant** argv) {
    GIGA_ASSERT(argc == 1, "Expected 1 argument.");
    GIGA_ASSERT(argv[0]->IsObject(), "Expected vector as argument.");
    
    Transform* tc = obj->AsObject<Transform>();
    Vector3* sv = argv[0]->AsObject<Vector3>();
    tc->Scale(sv->Get());
    
    sv->Set(tc->scaling);
    return(new Variant(sv));
}
