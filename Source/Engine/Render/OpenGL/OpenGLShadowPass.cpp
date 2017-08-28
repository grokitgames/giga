
#include <giga-engine.h>
#include <Render/OpenGL/OpenGL.hpp>

void OpenGLShadowPass::Initialize(int width, int height) {
    // Load shader
    m_shader = ShaderSystem::Find("depth.vs", "depth.fs", 0);
}

void OpenGLShadowPass::Render(Scene* scene) {
    PROFILE_START_AREA("Shadow Pass");
    
    // Reset counters
    m_drawCalls = 0;
    m_renderedTriangles = 0;
    
    GL_CHECK(glEnable(GL_DEPTH_TEST));
    GL_CHECK(glDepthFunc(GL_LEQUAL));
    
    glClearColor(FLT_MAX, FLT_MAX, FLT_MAX, FLT_MAX);
    
    // Clear our buffer
    GL_CHECK(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
    
    // Get our renderable mesh objects
    std::list<StaticMeshComponent*> renderList = scene->GetMeshes();
    
    // Get matrices
    matrix4 proj = m_camera->GetProjectionMatrix();
    m_view = m_camera->GetViewMatrix();
    m_viewproj = proj * m_view;
    
    // Get the viewing frustum of the camera
    Frustum viewFrustum = m_camera->GetViewFrustum();
    vector3 cameraPosition = m_camera->GetWorldPosition();
    
    // Loop over scene's renderable list and render
	std::list<StaticMeshComponent*>::iterator i = renderList.begin();
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
    GL_CHECK(glClearColor(0, 0, 0, 0));
    
    PROFILE_END_AREA("Shadow Pass");
}

void OpenGLShadowPass::RecursiveRender(StaticMeshComponent *mesh, matrix4 parent, Scene* scene, int index) {
    Mesh* renderable = mesh->GetMesh();
    
    std::vector<StaticMeshComponent*> children = mesh->GetChildren();
    int childCount = children.size();
    if (childCount) {
        for (int i = 0; i < childCount; i++) {
            RecursiveRender(children[i], parent, scene, index);
        }
        return;
    }
    
    // Use our depth shader instead of the normal mesh one
    m_shader->Bind();
    
    // Build our current model matrix
    glm::mat4 m = parent;
    glm::mat4 mv = m_view * m;
    glm::mat4 mvp = m_viewproj * m;
    
    // Bind our model-view-projection matrix
    m_shader->Set("mvpMatrix", mvp);
    
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
    vertexFormat->EnableVertexAttribs(VERTEX_ATTRIB_BONES);
    vertexFormat->EnableVertexAttribs(VERTEX_ATTRIB_BONE_WEIGHTS);
    
    // Either draw from vertex array or indices if defined
    if (indexBuffer == 0) {
        GL_CHECK(glDrawArrays(GL_TRIANGLES, 0, triCount * 3));
    }
    else {
        GL_CHECK(glDrawElements(GL_TRIANGLES, triCount * 3, GL_UNSIGNED_INT, 0));
    }
    
    vertexBuffer->Unbind();
    vertexFormat->DisableVertexAttribs();
    m_shader->Unbind();
    
    m_renderedTriangles += triCount;
    m_drawCalls++;
}
