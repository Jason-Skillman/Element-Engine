#pragma once

namespace Hazel {

	enum class RendererAPI {
		None = 0,
		OpenGL
	};
	
	class Renderer {
	private:
		static RendererAPI rendererApi;
		
	public:
		inline static RendererAPI GetRendererApi() {
			return rendererApi;
		}
	};
}
