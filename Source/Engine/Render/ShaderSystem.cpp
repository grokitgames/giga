
#include <giga-engine.h>

std::vector<ShaderSystem::CachedVariation*> ShaderSystem::m_cache;

ShaderSystem::~ShaderSystem() {
    for(size_t i = 0; i < m_cache.size(); i++) {
        delete m_cache[i];
    }
}

ShaderVariation* ShaderSystem::Find(std::string vert, std::string frag, ShaderAttributes* attribs) {
    // Look for an existing shader variation
    for(size_t i = 0; i < m_cache.size(); i++) {
        if(m_cache[i]->vert == vert &&
           m_cache[i]->frag == frag &&
           m_cache[i]->defines.size() == attribs->definitions.size()) {
            // Compare individual definitions
            bool match = true;
            
            for(size_t j = 0; j < m_cache[i]->defines.size(); i++) {
                if(attribs->definitions.find(m_cache[i]->defines[j]) == attribs->definitions.end()) {
                    match = false;
                }
            }
            
            if(match) {
                return(m_cache[i]->sv);
            }
        }
    }
    
    // Failing that, create a new shader variation
    CachedVariation* sv = new CachedVariation();
    sv->vert = vert;
    sv->frag = frag;
    
    std::map<std::string, bool>::iterator i = attribs->definitions.begin();
    for(i; i != attribs->definitions.end(); i++) {
        sv->defines.push_back(i->first);
    }
    
    // Get link to resource system
    ResourceSystem* resourceSystem = GetSystem<ResourceSystem>();
    
    // Load vertex and fragment shaders
    Shader* vshader = dynamic_cast<Shader*>(resourceSystem->LoadResource(vert, "Shader"));
    Shader* fshader = dynamic_cast<Shader*>(resourceSystem->LoadResource(frag, "Shader"));
    
    ShaderVariation* shader = new ShaderVariation();
    shader->SetVertexShader(vshader);
    shader->SetFragmentShader(fshader);
    
    // Set definitions
    i = attribs->definitions.begin();
    for(i; i != attribs->definitions.end(); i++) {
        shader->Define(i->first);
    }
    
    sv->sv = shader;
    
    m_cache.push_back(sv);
    
    return(shader);
}
