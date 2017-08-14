
#include <giga-engine.h>

void SpotLightComponent::Initialize() {
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
}

void SpotLightComponent::CreateDepthTextures(Scene* scene) {
    // Get our 3D texture
    Texture3D* texture = (Texture3D*)m_shadowMap;
    
    // Loop over each pass
    for(int i = 0; i < m_passes; i++) {
        // Set the output texture to the cubemap face
        m_framebuffer->Use();
        m_framebuffer->SetTexture(texture, FRAMEBUFFER_SLOT_0, texture->GetTarget(i));
        
        m_shadowPass->SetFrustum(&m_lightFrustums[i]);
        m_shadowPass->SetPosition(this->GetWorldPosition());
    }
}

SpotLightComponent* SpotLightComponent::Clone() {
    SpotLightComponent* sc = new SpotLightComponent();
    sc->m_attenuation = m_attenuation;
    sc->m_type = m_type;
    sc->m_color = m_color;
    sc->m_passes = m_passes;
    
    for(size_t i = 0; i < m_lightFrustums.size(); i++) {
        sc->m_lightFrustums.push_back(m_lightFrustums[i]);
    }
    
    sc->RecalculateMatrices();
    
    return(sc);
}

void SpotLightComponent::RecalculateMatrices() {
    
}
