
#include <giga-engine.h>

SceneComponent::SceneComponent() {
    m_transformComponent = 0;
    m_lit = false;
}

void SceneComponent::OnEntitySet() {
    // Check for a TransformComponent
    TransformComponent* tc = m_parent->FindComponent<TransformComponent>();
    if (tc == 0) {
        tc = new TransformComponent();
        m_parent->AddComponent(tc);
    }
    
    m_transformComponent = tc;
}

void SceneComponent::SetTransform(Transform* transform) {
    m_transform.scaling = transform->scaling;
    m_transform.position = transform->position;
    m_transform.rotation = transform->rotation;
}

vector3 SceneComponent::GetWorldPosition() const {
    vector3 position = m_transform.position;
    if (m_transformComponent) {
        Transform* worldTransform = m_transformComponent->GetTransform();
        position += worldTransform->position;
    }
    
    return(position);
}

void SceneComponent::SetWorldPosition(vector3 position) {
    if (m_transformComponent == 0) {
        m_transform.position = position;
        return;
    }
    
    Transform* worldTransform = m_transformComponent->GetTransform();
    vector3 relativePosition = worldTransform->position - position;
    m_transform.position = relativePosition;
}

vector3 SceneComponent::GetLocalPosition() const {
    return(m_transform.position);
}

void SceneComponent::SetLocalPosition(vector3 position) {
    m_transform.position = position;
}

quaternion SceneComponent::GetWorldRotation() const {
    quaternion rotation = m_transform.rotation;
    if (m_transformComponent) {
        Transform* worldTransform = m_transformComponent->GetTransform();
        quaternion worldRotation = worldTransform->rotation;
        rotation = worldRotation * rotation;
    }
    
    return(rotation);
}

void SceneComponent::SetWorldRotation(quaternion rotation) {
    if (m_transformComponent == 0) {
        m_transform.rotation = rotation;
        return;
    }
    
    Transform* worldTransform = m_transformComponent->GetTransform();
    quaternion worldRotation = worldTransform->rotation;
    quaternion localRotation = glm::inverse(worldRotation) * rotation;
    m_transform.rotation = localRotation;
}

quaternion SceneComponent::GetLocalRotation() const {
    return(m_transform.rotation);
}

void SceneComponent::SetLocalRotation(quaternion rotation) {
    m_transform.rotation = rotation;
}

vector3 SceneComponent::GetWorldScaling() const {
    vector3 scaling = m_transform.scaling;
    if (m_transformComponent) {
        Transform* worldTransform = m_transformComponent->GetTransform();
        scaling *= worldTransform->scaling;
    }
    
    return(scaling);
}

void SceneComponent::SetWorldScaling(vector3 scaling) {
    if (m_transformComponent == 0) {
        m_transform.scaling = scaling;
        return;
    }
    
    Transform* worldTransform = m_transformComponent->GetTransform();
    vector3 worldScaling = worldTransform->scaling;
    vector3 localScaling = worldScaling / scaling;
    m_transform.scaling = localScaling;
}

vector3 SceneComponent::GetLocalScaling() const {
    return(m_transform.scaling);
}

void SceneComponent::SetLocalScaling(vector3 scaling) {
    m_transform.scaling = scaling;
}

matrix4 SceneComponent::GetModelMatrix() {
    matrix4 mat = m_transform.GetModelMatrix();
    if (m_transformComponent) {
        mat = mat * m_transformComponent->GetTransform()->GetModelMatrix();
    }
    
    return(mat);
}

Variant* SceneComponent::GetTransform(Variant* obj) {
    SceneComponent* mi = obj->AsObject<SceneComponent>();
    Transform* transform = mi->GetTransform();
    
    return(new Variant(transform));
}

void SceneComponent::SetTransform(Variant* obj, Variant* val) {
    SceneComponent* mi = obj->AsObject<SceneComponent>();
    Transform* transform = val->AsObject<Transform>();
    
    mi->SetTransform(transform);
}
