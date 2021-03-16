#pragma once
#include "Element/Renderer/GraphicsContext.h"

struct GLFWwindow;

namespace Element {
	class OpenGLContext : public GraphicsContext {
	public:
		OpenGLContext(GLFWwindow* window);
		
	public:
		virtual void Init() override;
		virtual void SwapBuffers() override;

	private:
		GLFWwindow* windowHandle;
	};
}
