
#include <giga-engine.h>
#include <Render/OpenGL/OpenGL.hpp>

void OpenGLGBufferPass::Initialize(int windowWidth, int windowHeight) {
    m_windowWidth = windowWidth;
    m_windowHeight = windowHeight;
    
    RenderSystem* renderSystem = GetSystem<RenderSystem>();
    
    Framebuffer* gbufferFramebuffer = renderSystem->CreateFramebuffer();
    gbufferFramebuffer->Initialize(windowWidth, windowHeight);
    
    // Create our textures
    Texture2D* diffuseTexture = renderSystem->CreateTexture2D();
    diffuseTexture->Initialize(windowWidth, windowHeight, GL_RGB16F, GL_RGB);
    gbufferFramebuffer->AddTexture(diffuseTexture, GL_COLOR_ATTACHMENT0);
    
    Texture2D* positionTexture = renderSystem->CreateTexture2D();
    positionTexture->Initialize(windowWidth, windowHeight, GL_RGB16F, GL_RGB);
    gbufferFramebuffer->AddTexture(positionTexture, GL_COLOR_ATTACHMENT1);
    
    Texture2D* normalTexture = renderSystem->CreateTexture2D();
    normalTexture->Initialize(windowWidth, windowHeight, GL_RGB16F, GL_RGB);
    gbufferFramebuffer->AddTexture(normalTexture, GL_COLOR_ATTACHMENT2);
    
    Texture2D* indexTexture = renderSystem->CreateTexture2D();
    indexTexture->Initialize(windowWidth, windowHeight, GL_RGB16F, GL_RGB);
    gbufferFramebuffer->AddTexture(indexTexture, GL_COLOR_ATTACHMENT3);
    
    Texture2D* depthTexture = renderSystem->CreateTexture2D();
    depthTexture->Initialize(windowWidth, windowHeight, GL_DEPTH_COMPONENT32F, GL_DEPTH_COMPONENT);
    gbufferFramebuffer->AddTexture(depthTexture, GL_DEPTH_ATTACHMENT);
    
    m_framebuffers.push_back(gbufferFramebuffer);
}

Texture2D* OpenGLGBufferPass::GetDiffuseTexture() {
    return(m_framebuffers[0]->GetTexture(0));
}

Texture2D* OpenGLGBufferPass::GetNormalTexture() {
    return(m_framebuffers[0]->GetTexture(2));
}

Texture2D* OpenGLGBufferPass::GetPositionTexture() {
    return(m_framebuffers[0]->GetTexture(1));
}

Texture2D* OpenGLGBufferPass::GetDepthTexture() {
    return(m_framebuffers[0]->GetTexture(3));
}

void OpenGLGBufferPass::Render(Scene* scene) {
    PROFILE_START_AREA("Gbuffer Pass");
    
    // Reset counters
    m_drawCalls = 0;
    m_renderedTriangles = 0;
    
    // Bind our FBO and set the viewport to the proper size
    m_framebuffers[0]->Use();
    
    GL_CHECK(glEnable(GL_DEPTH_TEST));
    GL_CHECK(glDepthFunc(GL_LEQUAL));
    
    // Clear our buffer
    GL_CHECK(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
    
    // Clear index buffer
    float fValue = -1.0f;
    GL_CHECK(glClearBufferfv(GL_COLOR, 3, &fValue));
    
    // Get our renderable mesh objects
    std::vector<StaticMeshComponent*> renderList = scene->GetMeshes();
    
    // Get the camera
    CameraComponent* camera = scene->GetActiveCamera();
	if (camera == 0) {
		PROFILE_END_AREA("Gbuffer Pass");
		return;
	}
    
    // Update camera
    camera->SetViewport(m_windowWidth, m_windowHeight);
    
    // Get matrices
    matrix4 view = camera->GetViewMatrix();
    matrix4 proj = camera->GetProjectionMatrix();
    m_viewproj = proj * view;
    m_view = view;
    
    // Get the viewing frustum of the camera
    Frustum viewFrustum = camera->GetViewFrustum();
    vector3 cameraPosition = camera->GetWorldPosition();
    
    // Loop over scene's renderable list and render into g-buffer
	std::vector<StaticMeshComponent*>::iterator i = renderList.begin();
	int counter = 0;
	for (i; i != renderList.end(); i++) {
		// If this object doesn't have lighting applied, skip it (will cover next loop)
        if ((*i)->HasLighting() == false) {
            continue;
        }

		// Make sure this component is active
		if ((*i)->IsActive() == false) {
			continue;
		}
                
        matrix4 model = (*i)->GetModelMatrix();
                
        // Check to ensure it is inside the view frustum
        BoundingBox obb = (*i)->GetBoundingBox();
        if ((viewFrustum.Intersects(obb) == 0) && (obb.Inside(cameraPosition) == false)) {
            continue;
        }
                
        // Render recursively
        RecursiveRender((*i), model, scene, counter);
		counter++;
    }
    
    GL_CHECK(glDisable(GL_DEPTH_TEST));
    
    PROFILE_END_AREA("Gbuffer Pass");
}

void OpenGLGBufferPass::RecursiveRender(StaticMeshComponent *mesh, matrix4 parent, Scene* scene, int index) {
    Mesh* renderable = mesh->GetMesh();
    
    std::vector<StaticMeshComponent*> children = mesh->GetChildren();
    int childCount = children.size();
    if (childCount) {
        for (int i = 0; i < childCount; i++) {
            RecursiveRender(children[i], parent, scene, index);
        }
        return;
    }
    
    ShaderVariation* program = renderable->GetShader();
    program->Bind();
    
    // Set the index in the scene
    program->Set("sceneIndex", (float)index);
    
    // Build our current model matrix
    glm::mat4 m = parent;
    glm::mat4 mv = m_view * m;
    glm::mat4 mvp = m_viewproj * m;
    
    // Bind our model-view-projection matrix
    program->Set("mvpMatrix", mvp);
    program->Set("modelviewMatrix", mv);
    
    Material* material = renderable->GetMaterial();
    program->Set("materialID", (float)material->GetMaterial());
    
    // Bind our textures to their locations
    int texloc = 0;
    Texture2D* diffuseTexture = renderable->GetDiffuseTexture();
    Texture2D* normalTexture = renderable->GetNormalTexture();
    
    if (diffuseTexture) {
        program->Set("diffuseTexture", texloc);
        diffuseTexture->Bind(texloc);
        
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 16.0f);
        
        texloc++;
    }
    
    if (normalTexture) {
        program->Set("normalTexture", texloc);
        normalTexture->Bind(texloc);
        texloc++;
    }
    
    // Things we need
    VertexFormat* vertexFormat = renderable->GetVertexFormat();
    VertexBuffer* vertexBuffer = renderable->GetVertexBuffer();
    IndexBuffer* indexBuffer = renderable->GetIndexBuffer();
    int triCount = renderable->GetTriCount();
    
    // Bind the vertex attribute object, which will bind the buffers and attributes for us
    vertexFormat->Bind();
    vertexBuffer->Bind();
    
    // Enable attributes we might use (will only be enabled if present in vertex attrib array)
    vertexFormat->EnableVertexAttribs(VERTEX_ATTRIB_POSITION);
    vertexFormat->EnableVertexAttribs(VERTEX_ATTRIB_COLOR);
    vertexFormat->EnableVertexAttribs(VERTEX_ATTRIB_NORMAL);
    vertexFormat->EnableVertexAttribs(VERTEX_ATTRIB_TEXCOORD0);
    vertexFormat->EnableVertexAttribs(VERTEX_ATTRIB_BONES);
    vertexFormat->EnableVertexAttribs(VERTEX_ATTRIB_BONE_WEIGHTS);
    
    // Either draw from vertex array or indices if defined
    if (indexBuffer == 0) {
        GL_CHECK(glDrawArrays(GL_TRIANGLES, 0, triCount * 3));
    }
    else {
        GL_CHECK(glDrawElements(GL_TRIANGLES, triCount * 3, GL_UNSIGNED_INT, 0));
    }
    
    if (diffuseTexture) {
        diffuseTexture->Unbind();
    }
    
    if (normalTexture) {
        normalTexture->Unbind();
    }
    
    vertexBuffer->Unbind();
    vertexFormat->DisableVertexAttribs();
    program->Unbind();
    
    m_renderedTriangles += triCount;
    m_drawCalls++;
}
