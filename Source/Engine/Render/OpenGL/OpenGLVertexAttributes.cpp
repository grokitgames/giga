
#include <giga-engine.h>
#include <Render/OpenGL/OpenGL.hpp>

OpenGLVertexAttributes::~OpenGLVertexAttributes() {
    Destroy();
}

void OpenGLVertexAttributes::Create() {
    GL_CHECK(glGenVertexArrays(1, &m_vertexArray));
}

void OpenGLVertexAttributes::Destroy() {
    if (m_vertexArray) {
        GL_CHECK(glDeleteVertexArrays(1, &m_vertexArray));
    }
}

void OpenGLVertexAttributes::Bind() {
    GL_CHECK(glBindVertexArray(m_vertexArray));
}

void OpenGLVertexAttributes::Unbind() {
    GL_CHECK(glBindVertexArray(0));
}

void OpenGLVertexAttributes::EnableAttribute(unsigned int index, unsigned int size, unsigned int stride, unsigned int offset) {
    GL_CHECK(glEnableVertexAttribArray(index));
    GL_CHECK(glVertexAttribPointer(index, size, GL_FLOAT, GL_FALSE, sizeof(float) *  stride, (void*)(sizeof(float) * offset)));
}

void OpenGLVertexAttributes::DisableAttribute(unsigned int index) {
    GL_CHECK(glDisableVertexAttribArray(index));
}
