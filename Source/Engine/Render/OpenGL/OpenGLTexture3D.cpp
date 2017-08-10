
#include <giga-engine.h>
#include <Render/OpenGL/OpenGL.hpp>

void OpenGLTexture3D::Initialize(int width, int height, int format, int type) {
    if (m_texture) {
        GL_CHECK(glDeleteTextures(1, &m_texture));
        m_texture = 0;
    }
    
    GL_CHECK(glGenTextures(1, &m_texture));
    
    m_width = width;
    m_height = height;
    m_channels = 0;
    
    switch(type) {
        case GL_RED:
            m_channels = 1;
            break;
        case GL_RGB:
            m_channels = 3;
            break;
        case GL_RGBA:
            m_channels = 4;
            break;
        default:
            break;
    };
    
    GL_CHECK(glBindTexture(GL_TEXTURE_CUBE_MAP, m_texture));
    
    for (unsigned int i = 0; i < 6; ++i) {
        int slot = GL_TEXTURE_CUBE_MAP_POSITIVE_X + i;
        GL_CHECK(glTexImage2D(slot, 0, format, width, height, 0, type, GL_FLOAT, NULL));
    }
    
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    
    GL_CHECK(glBindTexture(GL_TEXTURE_CUBE_MAP, 0));
}

void OpenGLTexture3D::Bind(int slot) {
    GL_CHECK(glBindTexture(GL_TEXTURE_CUBE_MAP, m_texture));
}

void OpenGLTexture3D::Unbind() {
    GL_CHECK(glBindTexture(GL_TEXTURE_CUBE_MAP, 0));
}
