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
		static void Init();
		
		static void BeginScene(const OrthographicCamera& camera);
		static void EndScene();

		static void Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform = glm::mat4(1.0f));

		static void OnWindowResize(uint32_t width, uint32_t height);
		
		inline static RendererAPI::API GetAPI() {
			return RendererAPI::GetAPI();
		}
	};
}
