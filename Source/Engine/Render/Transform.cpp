
#include <giga-engine.h>

Transform::Transform() {
	position = vector3(0, 0, 0);
	rotation = quaternion(1, 0, 0, 0);
	scaling = vector3(1, 1, 1);
	m_component = 0;
	look = vector3(0, 0, -1);
	up = vector3(0, 1, 0);
	right = vector3(1, 0, 0);
	vlook = 0;
	vup = 0;
	vright = 0;
}

void Transform::Move(vector3 translation) {
    position += translation;
	NetworkSystem* networkSystem = GetSystem<NetworkSystem>();
	int tick = networkSystem->GetCurrentTick();

	char buffer[1000];
	sprintf(buffer, "%d: Translating to (%.2f, %.2f, %.2f)\n", tick, position.x, position.y, position.z);
	printf(buffer);
#ifdef WIN32
	OutputDebugString(buffer);
#endif
	if (m_component) m_component->MarkUpdated(true);
}

void Transform::Rotate(float degrees, vector3 axis) {
    quaternion delta = glm::angleAxis(glm::radians(degrees), axis);
    rotation = rotation * delta;
    
    look = rotation * glm::vec3(0.0f, 0.0f, -1.0f);
    right = rotation * glm::vec3(1.0f, 0.0f, 0.0f);
    up = rotation * glm::vec3(0.0f, 1.0f, 0.0f);

	if (m_component) m_component->MarkUpdated(true);
}

void Transform::Scale(vector3 scale) {
    scaling *= scale;

	if (m_component) m_component->MarkUpdated(true);
}

matrix4 Transform::GetModelMatrix() {
    UpdateModelMatrix();
    return(m_modelMatrix);
}

void Transform::UpdateModelMatrix() {
    matrix4 t = glm::translate(glm::mat4(1.0), position);
    matrix4 s = glm::scale(glm::mat4(1.0), scaling);
    matrix4 r = glm::mat4_cast(rotation);

	look = rotation * glm::vec3(0.0f, 0.0f, -1.0f);
	right = rotation * glm::vec3(1.0f, 0.0f, 0.0f);
	up = rotation * glm::vec3(0.0f, 1.0f, 0.0f);
    
    m_modelMatrix = t * r * s;
}

Variant* Transform::GetPosition(std::string var, Variant* obj) {
    Transform* tc = obj->AsObject<Transform>();
    Vector3* sv = new Vector3(tc->position);
    
    return(new Variant(sv));
}

void Transform::SetPosition(std::string var, Variant* obj, Variant* val) {
    Transform* tc = obj->AsObject<Transform>();
    Vector3* sv = val->AsObject<Vector3>();
    tc->position = sv->Get();

	if (tc->m_component) tc->m_component->MarkUpdated(true);
}

Variant* Transform::GetRotation(std::string var, Variant* obj) {
    Transform* tc = obj->AsObject<Transform>();
    Quaternion* sq = new Quaternion(tc->rotation);
    return(new Variant(sq));
}

void Transform::SetRotation(std::string var, Variant* obj, Variant* val) {
    Transform* tc = obj->AsObject<Transform>();
    Quaternion* sq = val->AsObject<Quaternion>();
    tc->rotation = sq->Get();

    tc->look = tc->rotation * glm::vec3(0.0f, 0.0f, -1.0f);
    tc->right = tc->rotation * glm::vec3(1.0f, 0.0f, 0.0f);
    tc->up = tc->rotation * glm::vec3(0.0f, 1.0f, 0.0f);
    
	if (tc->m_component) tc->m_component->MarkUpdated(true);
}

Variant* Transform::GetScaling(std::string var, Variant* obj) {
    Transform* tc = obj->AsObject<Transform>();
    Vector3* sv = new Vector3(tc->scaling);
    return(new Variant(sv));
}

void Transform::SetScaling(std::string var, Variant* obj, Variant* val) {
    Transform* tc = obj->AsObject<Transform>();
    Vector3* sv = val->AsObject<Vector3>();
    tc->scaling = sv->Get();

	if (tc->m_component) tc->m_component->MarkUpdated(true);
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

Variant* Transform::GetLook(std::string var, Variant* obj) {
    Transform* tc = obj->AsObject<Transform>();

	if (tc->vlook == 0) {
		tc->vlook = new Vector3();
	}
	tc->vlook->Set(tc->look);

    return(new Variant(tc->vlook));
}

Variant* Transform::GetUp(std::string var, Variant* obj) {
    Transform* tc = obj->AsObject<Transform>();

	if (tc->vup == 0) {
		tc->vup = new Vector3();
	}
	tc->vup->Set(tc->up);

	return(new Variant(tc->vup));
}

Variant* Transform::GetRight(std::string var, Variant* obj) {
    Transform* tc = obj->AsObject<Transform>();

	if (tc->vright == 0) {
		tc->vright = new Vector3();
	}
	tc->vright->Set(tc->right);

	return(new Variant(tc->vright));
}
