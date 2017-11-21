
#include <giga-engine.h>

CameraComponent::CameraComponent() {
    m_look = glm::vec3(0.0f, 0.0f, -1.0f);
    m_up = glm::vec3(0.0f, 1.0f, 0.0f);
    m_right = glm::vec3(1.0f, 0, 0.0f);
    m_width = 800;
    m_height = 600;
    
    m_fnear = 0.1f;
    m_ffar = 100.0f;
    m_fov = 45.0f;
    m_aspect = 1.3f;
    
    CalculateMatricesAndFrustum();
}

void CameraComponent::CalculateMatricesAndFrustum() {
    m_aspect = (float)m_width / m_height;
    
    m_transform.rotation = glm::rotation(vector3(0, 0, -1), m_look);
    m_right = GetWorldRotation() * glm::vec3(1.0f, 0.0f, 0.0f);
    
    // Update our view and projection matrices
    vector3 position = GetWorldPosition();
    /*m_look = GetWorldRotation() * glm::vec3(0.0f, 0.0f, -1.0f);
	
	m_up = GetWorldRotation() * glm::vec3(0.0f, 1.0f, 0.0f);*/

    m_view = glm::lookAt(position, position + m_look, m_up);
    m_projection = glm::perspective(glm::radians(m_fov), m_aspect, m_fnear, m_ffar);
    
    // Calculate the near and far plane points
    m_viewFrustum = CalculateFrustum(m_fnear, m_ffar);
}

Frustum CameraComponent::CalculateFrustum(float fnear, float ffar) {
    Frustum returnFrustum;
    
    // Calculate the near and far plane points
    float nearHeight = 2 * tan(m_fov / 2) * m_fnear;
    float nearWidth = nearHeight * m_aspect;
    float farHeight = 2 * tan(m_fov / 2) * m_ffar;
    float farWidth = farHeight * m_aspect;
    
    // And their centers
    vector3 position = GetWorldPosition();
    vector3 nearCenter = position + (m_look * m_fnear);
    vector3 farCenter = position + (m_look * m_ffar);
    
    returnFrustum.center = position + (m_look * (m_ffar / 2.0f));
    
    // And the actual coordinates
    returnFrustum.fnear[Frustum::COORD_BOTTOMLEFT] = nearCenter - (m_up * (nearHeight / 2.0f)) - (m_right * (nearWidth / 2.0f));
    returnFrustum.fnear[Frustum::COORD_BOTTOMRIGHT] = nearCenter - (m_up * (nearHeight / 2.0f)) + (m_right * (nearWidth / 2.0f));
    returnFrustum.fnear[Frustum::COORD_TOPLEFT] = nearCenter + (m_up * (nearHeight / 2.0f)) - (m_right * (nearWidth / 2.0f));
    returnFrustum.fnear[Frustum::COORD_TOPRIGHT] = nearCenter + (m_up * (nearHeight / 2.0f)) + (m_right * (nearWidth / 2.0f));
    
    returnFrustum.ffar[Frustum::COORD_BOTTOMLEFT] = farCenter - (m_up * (farHeight / 2.0f)) - (m_right * (farWidth / 2.0f));
    returnFrustum.ffar[Frustum::COORD_BOTTOMRIGHT] = farCenter - (m_up * (farHeight / 2.0f)) + (m_right * (farWidth / 2.0f));
    returnFrustum.ffar[Frustum::COORD_TOPLEFT] = farCenter + (m_up * (farHeight / 2.0f)) - (m_right * (farWidth / 2.0f));
    returnFrustum.ffar[Frustum::COORD_TOPRIGHT] = farCenter + (m_up * (farHeight / 2.0f)) + (m_right * (farWidth / 2.0f));
    
    // Then create our frustum planes
    returnFrustum.planes[Frustum::PLANE_NEAR].CreateFromPoints(m_viewFrustum.fnear[Frustum::COORD_BOTTOMLEFT], m_viewFrustum.fnear[Frustum::COORD_BOTTOMRIGHT], m_viewFrustum.fnear[Frustum::COORD_TOPLEFT]);
    returnFrustum.planes[Frustum::PLANE_FAR].CreateFromPoints(m_viewFrustum.ffar[Frustum::COORD_TOPLEFT], m_viewFrustum.ffar[Frustum::COORD_TOPRIGHT], m_viewFrustum.ffar[Frustum::COORD_BOTTOMLEFT]);
    returnFrustum.planes[Frustum::PLANE_LEFT].CreateFromPoints(m_viewFrustum.fnear[Frustum::COORD_BOTTOMLEFT], m_viewFrustum.fnear[Frustum::COORD_TOPLEFT], m_viewFrustum.ffar[Frustum::COORD_BOTTOMLEFT]);
    returnFrustum.planes[Frustum::PLANE_RIGHT].CreateFromPoints(m_viewFrustum.ffar[Frustum::COORD_TOPRIGHT], m_viewFrustum.fnear[Frustum::COORD_TOPRIGHT], m_viewFrustum.ffar[Frustum::COORD_BOTTOMRIGHT]);
    returnFrustum.planes[Frustum::PLANE_TOP].CreateFromPoints(m_viewFrustum.ffar[Frustum::COORD_TOPLEFT], m_viewFrustum.fnear[Frustum::COORD_TOPLEFT], m_viewFrustum.ffar[Frustum::COORD_TOPRIGHT]);
    returnFrustum.planes[Frustum::PLANE_BOTTOM].CreateFromPoints(m_viewFrustum.fnear[Frustum::COORD_BOTTOMLEFT], m_viewFrustum.ffar[Frustum::COORD_BOTTOMLEFT], m_viewFrustum.fnear[Frustum::COORD_BOTTOMRIGHT]);
    
    return returnFrustum;
}

void CameraComponent::SetLookVector(vector3 point) {
    m_look = point;
    
    CalculateMatricesAndFrustum();
}

void CameraComponent::SetUpVector(vector3 up) {
    m_up = up;
    
    CalculateMatricesAndFrustum();
}

void CameraComponent::SetNear(float fnear) {
    m_fnear = fnear;
    
    CalculateMatricesAndFrustum();
}

void CameraComponent::SetFar(float ffar) {
    m_ffar = ffar;
    
    CalculateMatricesAndFrustum();
}

void CameraComponent::SetFOV(float fov) {
    m_fov = fov;
    
    CalculateMatricesAndFrustum();
}

void CameraComponent::SetViewport(int width, int height) {
    m_width = width;
    m_height = height;
    
    this->SetAspectRatio((float)width / height);
}

void CameraComponent::SetAspectRatio(float ratio) {
    m_aspect = ratio;
    CalculateMatricesAndFrustum();
}

void CameraComponent::Copy(Component* component) {
	CameraComponent* cc = (CameraComponent*)component;

	cc->m_aspect = m_aspect;
	cc->m_fov = m_fov;
	cc->m_up = m_up;
	cc->m_ffar = m_ffar;
	cc->m_fnear = m_fnear;
	cc->m_look = m_look;
	cc->m_right = m_right;
	cc->m_width = m_width;
	cc->m_height = m_height;
	cc->CalculateMatricesAndFrustum();
}

Variant* CameraComponent::GetFOV(std::string var, Variant* obj) {
    CameraComponent* camera = obj->AsObject<CameraComponent>();
    return(new Variant(camera->m_fov));
}

void CameraComponent::SetFOV(std::string var, Variant* obj, Variant* val) {
    GIGA_ASSERT(val->IsFloat(), "Expecting first parameter to be a float.");
    CameraComponent* camera = obj->AsObject<CameraComponent>();
    camera->m_fov = val->AsFloat();
}

Variant* CameraComponent::SetViewport(Variant* obj, int argc, Variant** argv) {
    GIGA_ASSERT(argc == 2, "SetViewport function expects two arguments.");
    GIGA_ASSERT(argv[0]->IsInt(), "Expecting first parameter to be a float.");
    GIGA_ASSERT(argv[1]->IsInt(), "Expecting second parameter to be a float.");
    
    CameraComponent* camera = obj->AsObject<CameraComponent>();
    camera->SetViewport(argv[0]->AsInt(), argv[1]->AsInt());
    
    return(new Variant(0));
}

Variant* CameraComponent::GetNear(std::string var, Variant* obj) {
    CameraComponent* camera = obj->AsObject<CameraComponent>();
    return(new Variant(camera->m_fnear));
}

void CameraComponent::SetNear(std::string var, Variant* obj, Variant* val) {
    GIGA_ASSERT(val->IsFloat(), "Expecting first parameter to be a float.");
    CameraComponent* camera = obj->AsObject<CameraComponent>();
    camera->m_fnear = val->AsFloat();
}

Variant* CameraComponent::GetFar(std::string var, Variant* obj) {
    CameraComponent* camera = obj->AsObject<CameraComponent>();
    return(new Variant(camera->m_ffar));
}

void CameraComponent::SetFar(std::string var, Variant* obj, Variant* val) {
    GIGA_ASSERT(val->IsFloat(), "Expecting first parameter to be a float.");
    CameraComponent* camera = obj->AsObject<CameraComponent>();
    camera->m_ffar = val->AsFloat();
}

Variant* CameraComponent::GetLook(std::string var, Variant* obj) {
    CameraComponent* camera = obj->AsObject<CameraComponent>();
    Vector3* look = new Vector3(camera->GetLookVector());
    return(new Variant(look));
}

void CameraComponent::SetLook(std::string var, Variant* obj, Variant* val) {
    GIGA_ASSERT(val->IsObject(), "Expecting first parameter to be a Vector3 object.");
    CameraComponent* camera = obj->AsObject<CameraComponent>();
    Vector3* vec = val->AsObject<Vector3>();
    camera->SetLookVector(vec->Get());
}

Variant* CameraComponent::GetRight(std::string var, Variant* obj) {
    CameraComponent* camera = obj->AsObject<CameraComponent>();
    Vector3* right = new Vector3(camera->GetRightVector());
    return(new Variant(right));
}

Variant* CameraComponent::GetUp(std::string var, Variant* obj) {
    CameraComponent* camera = obj->AsObject<CameraComponent>();
    Vector3* up = new Vector3(camera->GetUpVector());
    return(new Variant(up));
}
