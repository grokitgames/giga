
#include <giga-engine.h>

VertexFormat::VertexFormat() {
    m_vertexArrayObject = 0;
}

VertexFormat::~VertexFormat() {
    if (m_vertexArrayObject) {
        m_vertexArrayObject->Destroy();
        delete m_vertexArrayObject;
    }
}

void VertexFormat::EnableVertexAttribs(int attribs) {
    std::vector<VertexAttrib*>::iterator i = m_attribs.begin();
    for (i; i != m_attribs.end(); i++) {
        VertexAttrib* attrib = (*i);
        if (attrib->index == attribs) {
            m_vertexArrayObject->EnableAttribute(attrib->index, attrib->components, attrib->stride, attrib->offset);
            break;
        }
    }
}

void VertexFormat::Bind() {
    if (m_vertexArrayObject == 0) {
        RenderSystem* renderSystem = GetSystem<RenderSystem>();
        
        m_vertexArrayObject = renderSystem->CreateVertexAttributes();
        m_vertexArrayObject->Create();
    }
    m_vertexArrayObject->Bind();
}

void VertexFormat::DisableVertexAttribs() {
    for (int i = 0; i < VERTEX_ATTRIB_LAST; i++) {
        m_vertexArrayObject->DisableAttribute(i);
    }
    
    m_vertexArrayObject->Unbind();
}

void VertexFormat::AddVertexAttribute(int index, int components, int stride, int offset) {
    VertexAttrib* att = new VertexAttrib;
    att->index = index;
    att->components = components;
    att->stride = stride;
    att->offset = offset;
    
    m_attribs.push_back(att);
}
