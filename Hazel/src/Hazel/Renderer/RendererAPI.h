#pragma once

#include <glm/glm.hpp>

#include "Hazel/Core.h"
#include "Hazel/Renderer/VertexArray.h"

namespace Hazel {
	class RendererAPI {
	public:
		enum class API {
			None = 0,
			OpenGL
		};

	private:
		static API api;
		
	public:
		virtual void SetClearColor(const glm::vec4& color) = 0;
		virtual void Clear() = 0;

		virtual void DrawIndexed(const Ref<VertexArray>& vertexArray) = 0;

		inline static API GetAPI() {
			return api;
		}
	};
}
