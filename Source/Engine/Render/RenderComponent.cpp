
#include <giga-engine.h>

RenderComponent::RenderComponent() {
    m_transformComponent = 0;
}

void RenderComponent::OnEntitySet() {
    // Check for a TransformComponent
    TransformComponent* tc = m_parent->FindComponent<TransformComponent>();
    if (tc == 0) {
        tc = new TransformComponent();
        m_parent->AddComponent(tc);
    }
    
    m_transformComponent = tc;
}

void RenderComponent::SetTransform(Transform* transform) {
    m_transform.scaling = transform->scaling;
    m_transform.position = transform->position;
    m_transform.rotation = transform->rotation;
    
    MarkUpdated(true);
}

vector3 RenderComponent::GetWorldPosition() const {
    vector3 position = m_transform.position;
    if (m_transformComponent) {
        Transform* worldTransform = m_transformComponent->GetTransform();
        position += worldTransform->position;
    }
    
    return(position);
}

void RenderComponent::SetWorldPosition(vector3 position) {
    if (m_transformComponent == 0) {
        m_transform.position = position;
        MarkUpdated(true);
        return;
    }
    
    Transform* worldTransform = m_transformComponent->GetTransform();
    vector3 relativePosition = worldTransform->position - position;
    m_transform.position = relativePosition;
    
    MarkUpdated(true);
}

vector3 RenderComponent::GetLocalPosition() const {
    return(m_transform.position);
}

void RenderComponent::SetLocalPosition(vector3 position) {
    m_transform.position = position;
    
    MarkUpdated(true);
}

quaternion RenderComponent::GetWorldRotation() const {
    quaternion rotation = m_transform.rotation;
    if (m_transformComponent) {
        Transform* worldTransform = m_transformComponent->GetTransform();
        quaternion worldRotation = worldTransform->rotation;
        rotation = worldRotation * rotation;
    }
    
    return(rotation);
}

void RenderComponent::SetWorldRotation(quaternion rotation) {
    if (m_transformComponent == 0) {
        m_transform.rotation = rotation;
        MarkUpdated(true);
        return;
    }
    
    Transform* worldTransform = m_transformComponent->GetTransform();
    quaternion worldRotation = worldTransform->rotation;
    quaternion localRotation = glm::inverse(worldRotation) * rotation;
    m_transform.rotation = localRotation;
    
    MarkUpdated(true);
}

quaternion RenderComponent::GetLocalRotation() const {
    return(m_transform.rotation);
}

void RenderComponent::SetLocalRotation(quaternion rotation) {
    m_transform.rotation = rotation;
    MarkUpdated(true);
}

vector3 RenderComponent::GetWorldScaling() const {
    vector3 scaling = m_transform.scaling;
    if (m_transformComponent) {
        Transform* worldTransform = m_transformComponent->GetTransform();
        scaling *= worldTransform->scaling;
    }
    
    return(scaling);
}

void RenderComponent::SetWorldScaling(vector3 scaling) {
    if (m_transformComponent == 0) {
        m_transform.scaling = scaling;
        MarkUpdated(true);
        return;
    }
    
    Transform* worldTransform = m_transformComponent->GetTransform();
    vector3 worldScaling = worldTransform->scaling;
    vector3 localScaling = worldScaling / scaling;
    m_transform.scaling = localScaling;
    
    MarkUpdated(true);
}

vector3 RenderComponent::GetLocalScaling() const {
    return(m_transform.scaling);
}

void RenderComponent::SetLocalScaling(vector3 scaling) {
    m_transform.scaling = scaling;
    MarkUpdated(true);
}

matrix4 RenderComponent::GetModelMatrix() {
    matrix4 mat = m_transform.GetModelMatrix();
    if (m_transformComponent) {
        mat = mat * m_transformComponent->GetTransform()->GetModelMatrix();
    }
    
    return(mat);
}

Variant* RenderComponent::GetTransform(Variant* obj) {
    RenderComponent* mi = obj->AsObject<RenderComponent>();
    Transform* transform = mi->GetTransform();
    
    return(new Variant(transform));
}

void RenderComponent::SetTransform(Variant* obj, Variant* val) {
    RenderComponent* mi = obj->AsObject<RenderComponent>();
    Transform* transform = val->AsObject<Transform>();
    
    mi->SetTransform(transform);
}
