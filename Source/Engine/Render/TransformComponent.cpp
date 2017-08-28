
#include <giga-engine.h>

void TransformComponent::SetDataMappings() {
    SetStorableObjectFieldMapping("position", &m_transform.position);
    SetStorableObjectFieldMapping("rotation", &m_transform.rotation);
    SetStorableObjectFieldMapping("scaling", &m_transform.scaling);
}

void TransformComponent::Interpolate(Component* component, float amount) {
    Transform interpolated;
    
    TransformComponent* tc = dynamic_cast<TransformComponent*>(component);
    Transform* newTransform = tc->GetTransform();

	interpolated.position = (m_transform.position * (1.0f - amount)) + (newTransform->position * amount);
	interpolated.rotation = glm::lerp(m_transform.rotation, newTransform->rotation, amount);
	interpolated.scaling = (m_transform.scaling * (1.0f - amount)) + (newTransform->scaling * amount);
    
    SetTransform(&interpolated);
}

TransformComponent* TransformComponent::Clone() {
    TransformComponent* tc = new TransformComponent();
    tc->SetTransform(&m_transform);
    
    return(tc);
}

void TransformComponent::SetTransform(Transform* transform) {
    m_transform.scaling = transform->scaling;
    m_transform.position = transform->position;
    m_transform.rotation = transform->rotation;
    
    MarkUpdated(true);
}

Variant* TransformComponent::GetTransform(std::string var, Variant* obj) {
    TransformComponent* tc = obj->AsObject<TransformComponent>();
    return(new Variant(tc->GetTransform()));
}

void TransformComponent::SetTransform(std::string var, Variant* obj, Variant* val) {
    TransformComponent* tc = obj->AsObject<TransformComponent>();
    Transform* t = val->AsObject<Transform>();
    tc->SetTransform(t);
}
