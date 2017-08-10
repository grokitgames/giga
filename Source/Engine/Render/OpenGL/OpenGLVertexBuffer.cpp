
#include <giga-engine.h>
#include <Render/OpenGL/OpenGL.hpp>

OpenGLVertexBuffer::OpenGLVertexBuffer() {
    m_count = 0;
    m_buffer = 0;
    m_vertexSize = 0;
    m_dynamic = false;
}

OpenGLVertexBuffer::~OpenGLVertexBuffer() {
    
}

void OpenGLVertexBuffer::Create(int count, int vertexSize, void *data, bool dynamic) {
    m_count = count;
    m_vertexSize = vertexSize;
    m_dynamic = dynamic;
    
    GL_CHECK(glGenBuffers(1, &m_buffer));
    GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, m_buffer));
    GL_CHECK(glBufferData(GL_ARRAY_BUFFER, sizeof(float) * m_vertexSize * m_count, data,
                          dynamic ? GL_STREAM_DRAW : GL_STATIC_DRAW));
}

void OpenGLVertexBuffer::SetData(void* data) {
    GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, m_buffer));
    GL_CHECK(glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * m_vertexSize * m_count, data));
}

void OpenGLVertexBuffer::Destroy() {
    if (m_buffer) {
        GL_CHECK(glDeleteBuffers(1, &m_buffer));
        m_buffer = 0;
        m_count = 0;
        m_vertexSize = 0;
    }
}

void OpenGLVertexBuffer::Bind() {
    GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, m_buffer));
    
    int size = 0;
    GL_CHECK(glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &size));
}

void OpenGLVertexBuffer::Unbind() {
    GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, 0));
}

float* OpenGLVertexBuffer::GetData(int &count) {
    std::vector<float> vertices;
    vertices.resize(m_count * m_vertexSize);
    
    float* ret = (float*)malloc(m_count * 3 * sizeof(float));
    GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, m_buffer));
    
    int size = 0;
    GL_CHECK(glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &size));
    GL_CHECK(glGetBufferSubData(GL_ARRAY_BUFFER, 0, m_vertexSize * m_count * sizeof(float), vertices.data()));
    
    for (int i = 0; i < m_count; i++) {
        int offset = i * 3;
        
        ret[offset + 0] = vertices[i * m_vertexSize + 0];
        ret[offset + 1] = vertices[i * m_vertexSize + 1];
        ret[offset + 2] = vertices[i * m_vertexSize + 2];
    }
    
    count = m_count;
    return(ret);
}
