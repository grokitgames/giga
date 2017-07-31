
#include <giga-engine.h>

RenderPass::~RenderPass() {
    for (size_t i = 0; i < m_framebuffers.size(); i++) {
        delete m_framebuffers[i];
    }
}

Framebuffer* RenderPass::GetFramebuffer(int index) {
    assert(index < m_framebuffers.size());
    return(m_framebuffers[index]);
}
