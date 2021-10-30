#pragma once

#include "RendererAPI.h"

namespace Element {

	class RenderCommand {
	private:
		static RendererAPI* rendererApi;
		
	public:
		static void Init() {
			rendererApi->Init();
		}

		static void SetClearColor(const glm::vec4& color) {
			rendererApi->SetClearColor(color);
		}

		static void Clear() {
			rendererApi->Clear();
		}

		static void DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount = 0) {
			rendererApi->DrawIndexed(vertexArray, indexCount);
		}

		static void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) {
			rendererApi->SetViewport(x, y, width, height);
		}
	};
}
