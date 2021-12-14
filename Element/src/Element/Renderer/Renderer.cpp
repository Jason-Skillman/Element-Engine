#include "pch.h"
#include "Renderer.h"

#include "Renderer2D.h"
#include "Platform/OpenGL/OpenGLShader.h"

namespace Element {

	Renderer::SceneData* Renderer::sceneData = new Renderer::SceneData();

	void Renderer::Init() {
		EL_PROFILE_FUNCTION();

		RenderCommand::Init();
		Renderer2D::Init();
	}

	void Renderer::BeginScene(const OrthographicCamera& camera) {
		EL_PROFILE_FUNCTION();

		sceneData->viewProjectionMatrix = camera.GetViewProjectionMatrix();
	}
	
	void Renderer::EndScene() {
		EL_PROFILE_FUNCTION();
	}
	
	void Renderer::Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform) {
		EL_PROFILE_FUNCTION();

		shader->Bind();

		//Todo: Remove
		std::dynamic_pointer_cast<Element::OpenGLShader>(shader)->UploadUniformMat4("u_ViewProjection", sceneData->viewProjectionMatrix);
		std::dynamic_pointer_cast<Element::OpenGLShader>(shader)->UploadUniformMat4("u_Transform", transform);
		
		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
	}

	void Renderer::OnWindowResize(uint32_t width, uint32_t height) {
		RenderCommand::SetViewport(0, 0, width, height);
	}
}
