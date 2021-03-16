#pragma once

#include "RendererAPI.h"

namespace Element {
	class RenderCommand {
	private:
		static RendererAPI* rendererApi;
		
	public:
		inline static void Init() {
			rendererApi->Init();
		}
		
		inline static void SetClearColor(const glm::vec4& color) {
			rendererApi->SetClearColor(color);
		}
		
		inline static void Clear() {
			rendererApi->Clear();
		}
		
		inline static void DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount = 0) {
			rendererApi->DrawIndexed(vertexArray, indexCount);
		}

		inline static void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) {
			rendererApi->SetViewport(x, y, width, height);
		}
	};
}
