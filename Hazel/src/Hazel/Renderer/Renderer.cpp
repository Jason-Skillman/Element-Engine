#include "hzpch.h"
#include "Renderer.h"

namespace Hazel {

	Renderer::SceneData* Renderer::sceneData = new Renderer::SceneData();

	void Renderer::BeginScene(const OrthographicCamera& camera) {
		sceneData->viewProjectionMatrix = camera.GetViewProjectionMatrix();
	}
	
	void Renderer::EndScene() {}
	
	void Renderer::Submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray) {
		vertexArray->Bind();
		shader->Bind();
		shader->SetUniformMat4fv("u_ViewProjection", sceneData->viewProjectionMatrix);
		RenderCommand::DrawIndexed(vertexArray);
	}
}
