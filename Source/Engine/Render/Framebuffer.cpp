
#include <giga-engine.h>
#include <Render/OpenGL.hpp>

Framebuffer::Framebuffer() {
    m_framebuffer = 0;
    m_complete = false;
    m_width = m_height = 0;
}

Framebuffer::~Framebuffer() {
    if (m_framebuffer) {
        GL_CHECK(glDeleteFramebuffers(1, &m_framebuffer));
        m_framebuffer = 0;
    }
    
    if (m_textures.size()) {
        for (int i = 0; i < m_textures.size(); i++) {
            delete m_textures[i];
        }
        
        m_textures.clear();
    }
}

void Framebuffer::Initialize(int width, int height) {
    m_width = width;
    m_height = height;
    
    if (m_framebuffer) {
        GL_CHECK(glDeleteFramebuffers(1, &m_framebuffer));
    }
    
    if (m_textures.size()) {
        for (int i = 0; i < m_textures.size(); i++) {
            delete m_textures[i];
        }
        
        m_textures.clear();
    }
    
    // Initialize our framebuffer
    GL_CHECK(glGenFramebuffers(1, &m_framebuffer));
    GL_CHECK(glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer));
}

void Framebuffer::AddTexture(Texture2D* texture, int type) {
    // Attach the texture to the FBO
    GL_CHECK(glFramebufferTexture2D(GL_FRAMEBUFFER, type, GL_TEXTURE_2D, texture->GetTexture(), 0));
    
    // Don't add depth textures to our framebuffer slots/counts
    if (type == GL_DEPTH_ATTACHMENT) {
        return;
    }
    
    m_textures.push_back(texture);
    m_slots.push_back(type);
    
    if (m_slots.size()) {
        GL_CHECK(glDrawBuffers(m_slots.size(), &m_slots[0]));
    }
    else {
        GL_CHECK(glDrawBuffers(m_slots.size(), GL_NONE));
    }
    
    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (status != GL_FRAMEBUFFER_COMPLETE) {
        ErrorSystem::Process(new Error(ERROR_WARN, "Unable to attach framebuffer."));
        assert(false); // Probably don't want to repeat this over and over again
    }
    
    m_complete = true;
}

void Framebuffer::SetTexture(Texture2D* texture, int type) {
    // Attach the texture to the FBO
    GL_CHECK(glFramebufferTexture2D(GL_FRAMEBUFFER, type, GL_TEXTURE_2D, texture->GetTexture(), 0));
}

void Framebuffer::Use() {
    assert(m_framebuffer > 0);
    
    // Attach the relevant buffers
    GL_CHECK(glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer));
    if (m_slots.size()) {
        GL_CHECK(glDrawBuffers(m_slots.size(), &m_slots[0]));
    }
    else {
        GL_CHECK(glDrawBuffers(m_slots.size(), GL_NONE));
    }
    
    if (m_complete == false) {
        GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
        if (status != GL_FRAMEBUFFER_COMPLETE) {
            ErrorSystem::Process(new Error(ERROR_WARN, "Unable to attach framebuffer."));
            assert(false); // Probably don't want to repeat this over and over again
        }
        
        m_complete = true;
    }
    
    GL_CHECK(glViewport(0, 0, m_width, m_height));
}
