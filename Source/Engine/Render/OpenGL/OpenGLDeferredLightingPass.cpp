
#include <giga-engine.h>
#include <Render/OpenGL/OpenGL.hpp>

void OpenGLDeferredLightingPass::InitializeShader() {
    // Load lighting shader
    ShaderSystem* shaderSystem = GetSystem<ShaderSystem>();
    m_shader = shaderSystem->Find("ortho.vs", "lighting.fs", new ShaderAttributes());
}

Texture2D* OpenGLDeferredLightingPass::GetLightingTexture() {
    return(m_framebuffers[0]->GetTexture(0));
}

void OpenGLDeferredLightingPass::Render(Scene* scene) {
    PROFILE_START_AREA("Deferred Lighting Pass");
    
    // Reset counters
    m_drawCalls = 0;
    m_renderedTriangles = 0;
    
    // Bind our FBO and set the viewport to the proper size
    m_framebuffers[0]->Use();
    
    GL_CHECK(glDisable(GL_DEPTH_TEST));
    
    // Clear our buffer
    GL_CHECK(glClear(GL_COLOR_BUFFER_BIT));
    
    GL_CHECK(glDisable(GL_BLEND));
    
    // Enable additive blending (accumulate lighting)
    GL_CHECK(glEnable(GL_BLEND));
    GL_CHECK(glBlendFunc(GL_ONE, GL_ONE));
    
    // Set ortho matrix
    m_shader->Bind();
    m_shader->Set("ortho", m_ortho);
    
    // Get active camera
    CameraComponent* camera = scene->GetActiveCamera();
    
    // Get view matrix
    matrix4 view = camera->GetViewMatrix();
    matrix4 worldViewInverse = glm::inverse(view);
    m_shader->Set("worldViewInverse", worldViewInverse);
    
    // Set camera position (specular mostly)
    m_shader->Set("cameraPosition", camera->GetWorldPosition());
    
    // Set textures
    m_diffuse->Bind(0);
    m_shader->Set("textureDiffuse", 0);
    
    m_position->Bind(1);
    m_shader->Set("textureNormal", 1);
    
    m_normal->Bind(2);
    m_shader->Set("texturePosition", 2);
    
    // Get our material texture
    MaterialSystem* materialSystem = GetSystem<MaterialSystem>();
    Texture* materialTexture = materialSystem->GetTexture();
    
    materialTexture->Bind(3);
    m_shader->Set("textureMaterialLookup", 3);
    
    // Make sure material texture does not filter
    GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
    GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
    
    // Bind vertex attrib object and buffer
    m_format->Bind();
    m_buffer->Bind();
    
    // Enable vertex attribs we want to use
    m_format->EnableVertexAttribs(VERTEX_ATTRIB_POSITION);
    m_format->EnableVertexAttribs(VERTEX_ATTRIB_TEXCOORD0);
    
    // Loop through each light
    std::list<LightComponent*> lights = scene->GetLights();
	std::list<LightComponent*>::iterator i = lights.begin();
	for (i; i != lights.end(); i++) {
        // Render
        GL_CHECK(glDrawArrays(GL_TRIANGLE_STRIP, 0, 4));
        
        m_drawCalls++;
        m_renderedTriangles += 2;
    }
    
    m_buffer->Unbind();
    m_format->DisableVertexAttribs();
    
    m_shader->Unbind();
    
    GL_CHECK(glDisable(GL_BLEND));
    
    PROFILE_END_AREA("Deferred Lighting Pass");
}
