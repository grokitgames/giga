
#include <giga-engine.h>
#include <Render/OpenGL/OpenGL.hpp>

void OpenGLDeferredRenderPass::InitializeShader() {
    // Load deferred shader
    ShaderSystem* shaderSystem = GetSystem<ShaderSystem>();
    m_shader = shaderSystem->Find("ortho.vs", "deferred.fs", new ShaderAttributes());
}

void OpenGLDeferredRenderPass::Render(Scene* scene) {
    PROFILE_START_AREA("Deferred Render Pass");
    
    // Reset counters
    m_drawCalls = 0;
    m_renderedTriangles = 0;
    
    // Bind our FBO and set the viewport to the proper size
    // m_framebuffers[0]->Use();
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    m_shader->Bind();
    
    GL_CHECK(glDisable(GL_DEPTH_TEST));
    
    // Clear our buffer
    GL_CHECK(glClear(GL_COLOR_BUFFER_BIT));
    
    // Set ortho matrix
    m_shader->Set("ortho", m_ortho);
    
    // Set ambient light
    m_shader->Set("ambientLight", scene->GetAmbientLight());
    
    // Set textures
    m_diffuse->Bind(0);
    m_shader->Set("textureDiffuse", 0);
    
    m_position->Bind(1);
    m_shader->Set("textureNormal", 1);
    
    m_normal->Bind(2);
    m_shader->Set("texturePosition", 2);
    
    m_lighting->Bind(3);
    m_shader->Set("textureLighting", 3);
    
    // Bind vertex attrib object and buffer
    m_format->Bind();
    m_buffer->Bind();
    
    // Enable vertex attribs we want to use
    m_format->EnableVertexAttribs(VERTEX_ATTRIB_POSITION);
    m_format->EnableVertexAttribs(VERTEX_ATTRIB_TEXCOORD0);
    
    // Render
    GL_CHECK(glDrawArrays(GL_TRIANGLE_STRIP, 0, 4));
    
    m_buffer->Unbind();
    m_format->DisableVertexAttribs();
    
    m_shader->Unbind();
    
    m_drawCalls++;
    m_renderedTriangles += 2;
    
    PROFILE_END_AREA("Deferred Render Pass");
}
