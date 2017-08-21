
#include <giga-engine.h>

LightComponent::LightComponent() {
    m_color = vector3(1.0f, 1.0f, 1.0f);
    m_attenuation = 10.0f;
    m_passes = 1;
    m_debugLight = 0;
    m_mesh = 0;
    m_shadowMap = 0;
    m_depthTexture = 0;
    m_shadowPass = 0;
    m_camera = 0;
}
