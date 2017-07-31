
#include <giga-engine.h>
#include <Render/OpenGL.hpp>

VertexAttribObject::~VertexAttribObject() {
    Destroy();
}

void VertexAttribObject::Create() {
    GL_CHECK(glGenVertexArrays(1, &m_vertexArray));
}

void VertexAttribObject::Destroy() {
    if (m_vertexArray) {
        GL_CHECK(glDeleteVertexArrays(1, &m_vertexArray));
    }
}

void VertexAttribObject::Bind() {
    GL_CHECK(glBindVertexArray(m_vertexArray));
}

void VertexAttribObject::EnableVertexAttribute(unsigned int index, unsigned int size, unsigned int stride, unsigned int offset) {
    GL_CHECK(glEnableVertexAttribArray(index));
    GL_CHECK(glVertexAttribPointer(index, size, GL_FLOAT, GL_FALSE, sizeof(float) *  stride, (void*)(sizeof(float) * offset)));
}

void VertexAttribObject::DisableVertexAttribute(unsigned int index) {
    GL_CHECK(glDisableVertexAttribArray(index));
}
