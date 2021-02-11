#include "hzpch.h"
#include "Renderer.h"

#include "Platform/OpenGL/OpenGLShader.h"

namespace Hazel {

	Renderer::SceneData* Renderer::sceneData = new Renderer::SceneData();

	void Renderer::Init() {
		RenderCommand::Init();
	}

	void Renderer::BeginScene(const OrthographicCamera& camera) {
		sceneData->viewProjectionMatrix = camera.GetViewProjectionMatrix();
	}
	
	void Renderer::EndScene() {}
	
	void Renderer::Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform) {
		vertexArray->Bind();
		shader->Bind();
		std::dynamic_pointer_cast<Hazel::OpenGLShader>(shader)->SetUniformMat4("u_ViewProjection", sceneData->viewProjectionMatrix);
		std::dynamic_pointer_cast<Hazel::OpenGLShader>(shader)->SetUniformMat4("u_Transform", transform);
		
		RenderCommand::DrawIndexed(vertexArray);
	}

	void Renderer::OnWindowResize(uint32_t width, uint32_t height) {
		RenderCommand::SetViewport(0, 0, width, height);
	}
}
