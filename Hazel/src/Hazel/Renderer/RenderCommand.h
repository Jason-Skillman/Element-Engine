#pragma once

#include "RendererAPI.h"

namespace Hazel {
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
		
		inline static void DrawIndexed(const Ref<VertexArray>& vertexArray) {
			rendererApi->DrawIndexed(vertexArray);
		}
	};
}
