#pragma once
#include "RenderCommand.h"

#include "OrthographicCamera.h"
#include "Shader.h"

namespace Hazel {

	class Renderer {
	private:
		struct SceneData {
			glm::mat4 viewProjectionMatrix;
		};

	public:
		static SceneData* sceneData;
		
	public:
		static void BeginScene(const OrthographicCamera& camera);
		static void EndScene();

		static void Submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray);
		
		inline static RendererAPI::API GetAPI() {
			return RendererAPI::GetAPI();
		}
	};
}
