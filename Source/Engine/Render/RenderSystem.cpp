
#include <giga-engine.h>
#include <Render/OpenGL.hpp>

RenderSystem::RenderSystem() {
    m_windowWidth = m_windowHeight = 0;
}

void RenderSystem::Initialize(int width, int height) {
    // Save
    m_windowWidth = width;
    m_windowHeight = height;
    
    // Log some stuff
    const unsigned char* version = glGetString(GL_VERSION);
    const unsigned char* driver = glGetString(GL_RENDERER);
    const unsigned char* glsl = glGetString(GL_SHADING_LANGUAGE_VERSION);
    
    Application::Log(MSGTYPE_INFO, "OpenGL Version", (char*)version);
    Application::Log(MSGTYPE_INFO, "Renderer", (char*)driver);
    Application::Log(MSGTYPE_INFO, "GLSL Version", (char*)glsl);
}

Variant* RenderSystem::GetWindowWidth(Variant* obj, int argc, Variant** argv) {
    RenderSystem* renderSystem = GetSystem<RenderSystem>();
    return(new Variant(renderSystem->m_windowWidth));
}

Variant* RenderSystem::GetWindowHeight(Variant* obj, int argc, Variant** argv) {
    RenderSystem* renderSystem = GetSystem<RenderSystem>();
    return(new Variant(renderSystem->m_windowHeight));
}
