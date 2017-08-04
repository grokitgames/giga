
#include <giga-engine.h>

void SpotLightComponent::Initialize() {
    
}

void SpotLightComponent::CreateDepthTextures(Scene* scene) {
    // Loop over each pass
    for(int i = 0; i < m_passes; i++) {
        
    }
}

SpotLightComponent* SpotLightComponent::Clone() {
    SpotLightComponent* sc = new SpotLightComponent();
    sc->m_attenuation = m_attenuation;
    sc->m_type = m_type;
    sc->m_color = m_color;
    sc->m_passes = m_passes;
    
    for(size_t i = 0; i < m_depthTextures.size(); i++) {
        sc->m_depthTextures.push_back(m_depthTextures[i]);
    }
    
    for(size_t i = 0; i < m_lightFrustums.size(); i++) {
        sc->m_lightFrustums.push_back(m_lightFrustums[i]);
    }
    
    sc->RecalculateMatrices();
    
    return(sc);
}

void SpotLightComponent::RecalculateMatrices() {
    
}
