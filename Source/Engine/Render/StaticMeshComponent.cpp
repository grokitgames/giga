
#include <giga-engine.h>

StaticMeshComponent::StaticMeshComponent() {
    m_mesh = 0;
    m_applyLighting = true;
	m_initialized = false;
}

StaticMeshComponent* StaticMeshComponent::Clone() {
	StaticMeshComponent* clone = (StaticMeshComponent*)Component::CreateComponent(this->GetTypeID());
    
    // Copy data
    clone->m_applyLighting = m_applyLighting;
    clone->m_obb = m_obb;
    clone->m_parent = m_parent;
    clone->SetTransform(this->GetTransform());
    clone->m_mesh = m_mesh;
    
    // Create new instances of child objects
    for (size_t i = 0; i < m_children.size(); i++) {
        clone->m_children.push_back((StaticMeshComponent*)m_children[i]->Clone());
    }
    
    return(clone);
}

void StaticMeshComponent::Instantiate(Mesh *mesh) {
	if (m_initialized && (m_mesh = mesh)) {
		return;
	}

    m_mesh = mesh;
    
    // Create static mesh components for child mesh objects (so they can be moved, rotated, etc.)
    std::vector<Mesh*> children = mesh->GetChildren();
    for (size_t i = 0; i < children.size(); i++) {
        StaticMeshComponent* m = new StaticMeshComponent();
        
        m->Instantiate(children[i]);
        m_children.push_back((StaticMeshComponent*)m);
    }
    
    UpdateBoundingBox();
	m_initialized = true;
}

void StaticMeshComponent::Interpolate(Component* component, float amount) {
	Transform interpolated;

	StaticMeshComponent* tc = dynamic_cast<StaticMeshComponent*>(component);
	Transform* newTransform = tc->GetTransform();

	interpolated.position = (m_transform.position * (1.0f - amount)) + (newTransform->position * amount);
	interpolated.rotation = glm::lerp(m_transform.rotation, newTransform->rotation, amount);
	interpolated.scaling = (m_transform.scaling * (1.0f - amount)) + (newTransform->scaling * amount);

	SetTransform(&interpolated);
}

void StaticMeshComponent::UpdateBoundingBox() {
    m_obb.min.x = m_obb.min.y = m_obb.min.z = FLT_MAX;
    m_obb.max.x = m_obb.max.y = m_obb.max.z = -FLT_MAX;
    
    BoundingBox aabb = m_mesh->GetBoundingBox();
    for (int i = 0; i < 8; i++) {
        m_obb.points[i] = (m_transform.rotation * (aabb.points[i] * m_transform.scaling)) + m_transform.position;
        
        m_obb.min.x = fmin(m_obb.min.x, m_obb.points[i].x);
        m_obb.min.y = fmin(m_obb.min.y, m_obb.points[i].y);
        m_obb.min.z = fmin(m_obb.min.z, m_obb.points[i].z);
        
        m_obb.max.x = fmax(m_obb.max.x, m_obb.points[i].x);
        m_obb.max.y = fmax(m_obb.max.y, m_obb.points[i].y);
        m_obb.max.z = fmax(m_obb.max.z, m_obb.points[i].z);
    }
}

void StaticMeshComponent::SetDataMappings() {
    SetStorableObjectFieldMapping("position", &m_transform.position);
    SetStorableObjectFieldMapping("rotation", &m_transform.rotation);
    SetStorableObjectFieldMapping("scaling", &m_transform.scaling);
    SetStorableObjectFieldMapping("mesh", (ResourceObject**)&m_mesh);
}

void StaticMeshComponent::UpdateFromDataMappings() {
	this->Instantiate(m_mesh);
}

Variant* StaticMeshComponent::Instantiate(Variant* obj, int argc, Variant** argv) {
    GIGA_ASSERT(argc == 1, "Instantiate expects one argument.");
    GIGA_ASSERT(argv[0]->IsObject(), "First parameter should be a Mesh object.");
    
    StaticMeshComponent* sm = obj->AsObject<StaticMeshComponent>();
    Mesh* mesh = argv[0]->AsObject<Mesh>();
    sm->Instantiate(mesh);
    
    return(new Variant(0));
}
