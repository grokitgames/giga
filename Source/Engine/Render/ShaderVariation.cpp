
#include <giga-engine.h>

ShaderVariation::ShaderVariation() {
    m_vertexShader = 0;
    m_fragmentShader = 0;
    m_attribs = 0;
}

ShaderVariation::~ShaderVariation() {
    if(m_vertexShader) {
        delete m_vertexShader;
    }
    
    if(m_fragmentShader) {
        delete m_fragmentShader;
    }
    
    for(size_t i = 0; i < m_vars.size(); i++) {
        delete m_vars[i];
    }
}

void ShaderVariation::Define(std::string define) {
    if(m_attribs == 0) {
        m_attribs = new ShaderAttributes();
    }
    
    m_attribs->Add(define);
}
