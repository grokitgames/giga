
#include <giga-engine.h>

void SpotLightComponent::Initialize() {
    m_passes = 6;
    
    // Get link to render system
    RenderSystem* renderSystem = GetSystem<RenderSystem>();
    
    // Create cubemap shadow texture
    m_shadowMap = renderSystem->CreateTexture3D();
    m_shadowMap->Initialize(512, 512, COLOR_RED16F, COLOR_RED);
    
    // Create depth texture (one used over and over for each pass)
    m_depthTexture = renderSystem->CreateTexture2D();
    m_depthTexture->Initialize(512, 512, COLOR_DEPTH_COMPONENT32, COLOR_DEPTH_COMPONENT);
    
    m_framebuffer = renderSystem->CreateFramebuffer();
    m_framebuffer->AddTexture(m_depthTexture, FRAMEBUFFER_SLOT_0);
    m_framebuffer->AddTexture(m_depthTexture, FRAMEBUFFER_SLOT_DEPTH);
    
    // Create render pass
    m_shadowPass = renderSystem->CreateShadowPass();
    m_shadowPass->Initialize(512, 512);
    
    // Initialize "camera"
    m_camera = new CameraComponent();
    m_camera->SetNear(0.1f);
    
    // Right
    m_vectors[0].look = vector3(1, 0, 0);
    m_vectors[0].up = vector3(0, 1, 0);
    
    // Left
    m_vectors[1].look = vector3(-1, 0, 0);
    m_vectors[1].up = vector3(0, 1, 0);
    
    // Up
    m_vectors[2].look = vector3(0, 1, 0);
    m_vectors[2].up = vector3(0, 0, 1);
    
    // Down
    m_vectors[3].look = vector3(0, -1, 0);
    m_vectors[3].up = vector3(0, 0, -1);
    
    // Backward
    m_vectors[4].look = vector3(0, 0, 1);
    m_vectors[4].up = vector3(0, 1, 0);
    
    // Forward
    m_vectors[5].look = vector3(0, 0, -1);
    m_vectors[5].up = vector3(0, 1, 0);
}

void SpotLightComponent::CreateDepthTextures(Scene* scene) {
    // Get our 3D texture
    Texture3D* texture = (Texture3D*)m_shadowMap;
    
    // Get world position
    vector3 position = GetWorldPosition();
    
    // Update "camera"
    m_camera->SetWorldPosition(position);
    m_camera->SetFar(m_attenuation);
    
    // Loop over each pass
    for(int i = 0; i < m_passes; i++) {
        // Set the output texture to the cubemap face
        m_framebuffer->Use();
        m_framebuffer->SetTexture(texture, FRAMEBUFFER_SLOT_0, texture->GetTarget(i));
        
        // Calculate frustum and matrices
        m_camera->SetLookVector(m_vectors[i].look);
        m_camera->SetUpVector(m_vectors[i].up);
        
        // Set "camera" and draw
        m_shadowPass->SetView(m_camera);
        m_shadowPass->Render(scene);
    }
}

void SpotLightComponent::Copy(Component* component) {
    SpotLightComponent* sc = (SpotLightComponent*)component;
    sc->m_attenuation = m_attenuation;
    sc->m_type = m_type;
    sc->m_color = m_color;
    sc->m_passes = m_passes;
    sc->m_camera = (CameraComponent*)m_camera->Clone();
    
    sc->RecalculateMatrices();
}

void SpotLightComponent::RecalculateMatrices() {

}

void SpotLightComponent::SetDataMappings() {
    SetStorableObjectFieldMapping("position", &m_transform.position);
    SetStorableObjectFieldMapping("color", &m_color);
    SetStorableObjectFieldMapping("attenuation", &m_attenuation);
}

void SpotLightComponent::UpdateFromDataMappings() {
    this->Initialize();
}
