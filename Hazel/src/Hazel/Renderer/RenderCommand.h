#pragma once

#include "RendererAPI.h"

namespace Hazel {
	class RenderCommand {
	private:
		static RendererAPI* rendererApi;
		
	public:
		inline static void SetClearColor(const glm::vec4& color) {
			rendererApi->SetClearColor(color);
		}
		
		inline static void Clear() {
			rendererApi->Clear();
		}
		
		inline static void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray) {
			rendererApi->DrawIndexed(vertexArray);
		}
	};
}
